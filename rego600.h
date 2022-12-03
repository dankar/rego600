#ifndef _REGO600_H_
#define _REGO600_H_

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "rego600_definitions.h"
#include <cstdint>
#include <optional>

// Struct for all the sensor temperatures that we extract.
struct temperatures_t {
    std::optional<float> radiator_return;
    std::optional<float> outdoor;
    std::optional<float> indoor;
    std::optional<float> warm_water;
    std::optional<float> radiator_forward;
    std::optional<float> heat_fluid_in;
    std::optional<float> heat_fluid_out;
    std::optional<float> cold_fluid_in;
    std::optional<float> cold_fluid_out;
};

// Struct for all the target temperatures that we extract.
struct targets_t {
    std::optional<float> radiator_return_target;
    std::optional<float> radiator_return_on;
    std::optional<float> radiator_return_off;
    std::optional<float> warm_water_target;
    std::optional<float> warm_water_on;
    std::optional<float> warm_water_off;
    std::optional<float> add_heat_percentage;
};

// This class implements the protocol for communicating with
// Rego 600 based heat pumps.
class rego600 : public esphome::Component {
private:
    static constexpr uint32_t m_timeout = 500;

    esphome::uart::UARTDevice    m_uart;
    static constexpr const char* TAG = "Rego600";

    std::optional<std::vector<uint8_t>> read_serial(size_t len);
    bool                                write_serial(const std::vector<uint8_t>& data);

    int16_t read_integer(const uint8_t* data);

    float                  integer_to_float(int16_t i);
    int16_t                float_to_integer(float f);
    void                   write_integer(int16_t val, uint8_t* data);
    uint8_t                calculate_checksum(const uint8_t* data, size_t len);
    bool                   read_ack_response();
    std::optional<int16_t> read_response();
    void                   flush_receive();

    enum response_type_t { ONE_BYTE_RESPONSE, FIVE_BYTE_RESPONSE };

    std::optional<int16_t> run_command(target_t target, operation_t operation, uint16_t reg, uint16_t value, response_type_t response_type);

    std::optional<float> read_sensor_register(sensor_register_t reg);
    bool                 write_button_register(front_panel_button_t reg, uint16_t val);
    std::optional<bool>  read_light_register(front_panel_light_t reg);

public:
    // Constructor only takes a pre-initialized UART component.
    // The user must initialize it with the correct settings for
    // communicating with the pump (via the chosen circuit).
    rego600(esphome::uart::UARTComponent* uart)
        : m_uart(uart)
    {
    }

    // Read a status register (binary on/off) and maybe return value.
    std::optional<int16_t> read_status(status_register_t reg);
    // Read a floating point sensor (values from these sensors are
    // encoded as int(value*10)
    std::optional<float> read_decimal_sensor(sensor_register_t reg);
    // Try to read all sensors and return a struct with the result.
    temperatures_t read_all_sensors();
    // Try to read all target settings and return a struct with the result.
    targets_t read_all_targets();
    // Send command to press a button on the front panel.
    bool press_button(front_panel_button_t button);
    // Read the status of a front panel light (on/off).
    std::optional<bool> read_panel_light(front_panel_light_t reg);
    // Read and write the control registers that provide us with settings.
    std::optional<float> read_control_register(control_register_t reg);
    bool                 write_control_register(control_register_t reg, float val);
};

#endif
