#ifndef _REGO600_STATUS_H_
#define _REGO600_STATUS_H_

#include "esphome.h"
#include "rego600.h"
#include "rego600_config.h"
#include "rego600_definitions.h"
#include <tuple>
#include <vector>

class rego600_status : public esphome::PollingComponent {
private:
    using status_map_t = std::vector<std::tuple<status_register_t, esphome::binary_sensor::BinarySensor*>>;
    using light_map_t  = std::vector<std::tuple<front_panel_light_t, esphome::binary_sensor::BinarySensor*>>;
    rego600*                     m_rego600;
    static constexpr const char* TAG = "Rego600Status";

public:
    esphome::binary_sensor::BinarySensor* ground_loop_pump   = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* compressor         = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* heat_cartridge_3kw = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* heat_cartridge_6kw = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* radiator_pump      = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* heat_carrier_pump  = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* three_way_valve    = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* alarm              = new esphome::binary_sensor::BinarySensor();

    esphome::binary_sensor::BinarySensor* led_power    = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* led_pump     = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* led_add_heat = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* led_boiler   = new esphome::binary_sensor::BinarySensor();
    esphome::binary_sensor::BinarySensor* led_alarm    = new esphome::binary_sensor::BinarySensor();

    const status_map_t m_status_map { { GROUND_LOOP_PUMP, ground_loop_pump }, { COMPRESSOR, compressor }, { HEAT_CARTRIDGE_3KW, heat_cartridge_3kw },
        { HEAT_CARTRIDGE_6KW, heat_cartridge_6kw }, { RADIATOR_PUMP, radiator_pump }, { HEAT_CARRIER_PUMP, heat_carrier_pump },
        { THREE_WAY_VALVE, three_way_valve }, { ALARM, alarm } };

    const light_map_t m_light_map { { LED_POWER, led_power }, { LED_PUMP, led_pump }, { LED_ADD_HEAT, led_add_heat }, { LED_BOILER, led_boiler },
        { LED_ALARM, led_alarm } };

    rego600_status(rego600* rego)
        : PollingComponent(STATUS_REGISTER_INTERVAL)
        , m_rego600(rego)
    {
    }

    void update() override final;
};

#endif
