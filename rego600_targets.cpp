#include "rego600_targets.h"
#include "esphome.h"
#include "rego600.h"
#include "rego600_definitions.h"

void rego600_targets::update()
{
    targets_t t = m_rego600->read_all_targets();

    if (t.radiator_return_target) {
        radiator_return_target->publish_state(t.radiator_return_target.value());
    }

    if (t.radiator_return_on) {
        radiator_return_on->publish_state(t.radiator_return_on.value());
    }

    if (t.radiator_return_off) {
        radiator_return_off->publish_state(t.radiator_return_off.value());
    }

    if (t.warm_water_target) {
        warm_water_target->publish_state(t.warm_water_target.value());
    }

    if (t.warm_water_on) {
        warm_water_on->publish_state(t.warm_water_on.value());
    }

    if (t.warm_water_off) {
        warm_water_off->publish_state(t.warm_water_off.value());
    }

    if (t.add_heat_percentage) {
        add_heat_percentage->publish_state(t.add_heat_percentage.value());
    }
}
