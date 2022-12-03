#include "rego600_control.h"

void rego600_number::control(float value)
{
    m_rego600->write_control_register(m_register, value);

    // Ugly. Can we schedule a read instead?
    esphome::delay(1000);

    refresh();
}

void rego600_number::refresh()
{
    auto opt = m_rego600->read_control_register(m_register);
    if (opt.has_value()) {
        this->publish_state(opt.value());
    }
}

void rego600_control::update()
{
    heat_curve->refresh();
    heat_curve_fine_adjust->refresh();
    indoor_temperature_target->refresh();
    curve_influence_by_indoor_temperature->refresh();
    curve_adjust_at_plus_20->refresh();
    curve_adjust_at_plus_15->refresh();
    curve_adjust_at_plus_10->refresh();
    curve_adjust_at_plus_05->refresh();
    curve_adjust_at_plus_00->refresh();
    curve_adjust_at_minus_05->refresh();
    curve_adjust_at_minus_10->refresh();
    curve_adjust_at_minus_15->refresh();
    curve_adjust_at_minus_20->refresh();
    curve_adjust_at_minus_25->refresh();
    curve_adjust_at_minus_30->refresh();
    curve_adjust_at_minus_35->refresh();
    heat_curve_coupling->refresh();
}
