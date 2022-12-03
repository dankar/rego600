#include "rego600_temperature_sensors.h"

void rego600_temperature_sensors::update()
{
    temperatures_t t = m_rego600->read_all_sensors();

    if (t.radiator_return) {
        radiator_return->publish_state(t.radiator_return.value());
    }

    if (t.indoor) {
        indoor->publish_state(t.indoor.value());
    }

    if (t.outdoor) {
        outdoor->publish_state(t.outdoor.value());
    }

    if (t.warm_water) {
        warm_water->publish_state(t.warm_water.value());
    }

    if (t.radiator_forward) {
        radiator_forward->publish_state(t.radiator_forward.value());
    }

    if (t.heat_fluid_in) {
        heat_fluid_in->publish_state(t.heat_fluid_in.value());
    }

    if (t.heat_fluid_out) {
        heat_fluid_out->publish_state(t.heat_fluid_out.value());
    }

    if (t.cold_fluid_in) {
        cold_fluid_in->publish_state(t.cold_fluid_in.value());
    }

    if (t.cold_fluid_out) {
        cold_fluid_out->publish_state(t.cold_fluid_out.value());
    }
}
