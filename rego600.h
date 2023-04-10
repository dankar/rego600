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
    std::optional<float> compressor;
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
    static constexpr uint32_t m_timeout         = 500;
    static constexpr uint32_t m_timeout_holdoff = 500;
    uint32_t m_last_timeout    = 0;

    esphome::uart::UARTDevice    m_uart;
    static constexpr const char* TAG = "Rego600";

    std::optional<std::vector<uint8_t>> read_serial(size_t len);
    bool                                write_serial(const std::vector<uint8_t>& data);

    int16_t read_integer(const uint8_t* data);

    float                               integer_to_float(int16_t i);
    int16_t                             float_to_integer(float f);
    void                                write_integer(int16_t val, uint8_t* data);
    uint8_t                             calculate_checksum(const uint8_t* data, size_t len);
    bool                                read_ack_response();
    std::optional<int16_t>              read_response();
    std::optional<std::vector<uint8_t>> read_long_response();
    void                                flush_receive();

    enum response_type_t { ONE_BYTE_RESPONSE, FIVE_BYTE_RESPONSE, FOURTYTWO_BYTE_RESPONSE };

    // This template nightmare will return different types depending on the response
    // template parameter:
    //
    // ONE_BYTE_RESPONSE:        bool
    // FIVE_BYTE_RESPONSE:       std::optional<int16_t>
    // FOURTY_TWO_BYTE_RESPONSE: std::optional<std::vector<uint8_t>>
    template <response_type_t response> decltype(auto) run_command(target_t target, operation_t operation, uint16_t reg, uint16_t value)
    {
        command_t                           cmd;
        std::optional<int16_t>              result;
        std::optional<std::vector<uint8_t>> long_result;

        cmd.target    = target;
        cmd.operation = operation;
        write_integer(reg, cmd.reg);
        write_integer(value, cmd.value);
        cmd.checksum = calculate_checksum(cmd.reg, 6);

        std::vector<uint8_t> data(sizeof(command_t));

        memcpy(&data[0], &cmd, sizeof(command_t));

        flush_receive();

        if (write_serial(data)) {
            if constexpr (response == ONE_BYTE_RESPONSE) {
                if (read_ack_response()) {
                    result = 1;
                }
            } else if constexpr (response == FIVE_BYTE_RESPONSE) {
                result = read_response();
            } else if constexpr (response == FOURTYTWO_BYTE_RESPONSE) {
                long_result = read_long_response();
            }
        }

        flush_receive();

        if constexpr (response == ONE_BYTE_RESPONSE) {
            if (result.has_value()) {
                return true;
            } else {
                return false;
            }
        } else if constexpr (response == FOURTYTWO_BYTE_RESPONSE) {
            return long_result;
        } else if constexpr (response == FIVE_BYTE_RESPONSE) {
            return result;
        }
    }

    std::optional<float> read_sensor_register(sensor_register_t reg);
    bool                 write_button_register(front_panel_button_t reg, uint16_t val);
    std::optional<bool>  read_light_register(front_panel_light_t reg);

    std::optional<std::vector<uint8_t>> read_display_line(uint8_t line);

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

    std::optional<std::vector<std::string>> read_display();
};

#endif
