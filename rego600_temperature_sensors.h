#ifndef _REGO600_TEMPERATURE_SENSORS_H_
#define _REGO600_TEMPERATURE_SENSORS_H_

#include "esphome.h"
#include "rego600.h"
#include "rego600_config.h"

class rego600_temperature_sensors : public esphome::PollingComponent {
private:
    rego600*                     m_rego600;
    static constexpr const char* TAG = "Rego600";

public:
    esphome::sensor::Sensor* radiator_return  = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* outdoor          = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* indoor           = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* warm_water       = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* radiator_forward = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* heat_fluid_in    = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* heat_fluid_out   = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* cold_fluid_in    = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* cold_fluid_out   = new esphome::sensor::Sensor();
    esphome::sensor::Sensor* compressor       = new esphome::sensor::Sensor();

    rego600_temperature_sensors(rego600* rego)
        : PollingComponent(TEMPERATURE_REGISTER_INTERVAL)
        , m_rego600(rego)
    {
    }

    void setup() override final { }

    void update() override final;
};

#endif
