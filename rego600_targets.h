#ifndef _REGO600_TARGETS_H_
#define _REGO600_TARGETS_H_

#include "esphome.h"
#include "rego600.h"
#include "rego600_config.h"
#include "rego600_definitions.h"

class rego600_targets : public esphome::PollingComponent {
private:
    rego600*                     m_rego600;
    static constexpr const char* TAG = "Rego600Targets";

public:
    esphome::sensor::Sensor* radiator_return_target = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* radiator_return_on     = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* radiator_return_off    = new esphome::sensor::Sensor();

    esphome::sensor::Sensor* warm_water_target = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* warm_water_on     = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* warm_water_off    = new esphome::sensor::Sensor();

    esphome::sensor::Sensor* add_heat_percentage = new esphome::sensor::Sensor();

    rego600_targets(rego600* rego)
        : PollingComponent(TARGET_REGISTER_INTERVAL)
        , m_rego600(rego)
    {
    }

    void setup() override final { }

    void update() override final;
};

#endif
