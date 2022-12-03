#ifndef _REGO600_CONTROL_H_
#define _REGO600_CONTROL_H_

#include "esphome.h"
#include "rego600.h"
#include "rego600_config.h"
#include "rego600_definitions.h"
#include <optional>

using namespace esphome;
using namespace esphome::number;

class rego600_number : public esphome::number::Number, esphome::Component {
private:
    rego600*           m_rego600;
    control_register_t m_register;

public:
    rego600_number(
        const char* name, rego600* rego, control_register_t reg, float max, float min, const std::optional<std::string>& unit = std::nullopt)
        : m_rego600(rego)
        , m_register(reg)
    {
        traits.set_min_value(min);
        traits.set_max_value(max);
        traits.set_step(0.1f);
        traits.set_mode(NUMBER_MODE_SLIDER);
        if (unit.has_value()) {
            traits.set_unit_of_measurement(unit.value().c_str());
        }
        set_name(name);
        App.register_number(this);
    }
    void control(float value) override final;
    void refresh();
};

class rego600_control : public esphome::PollingComponent {
private:
    rego600*                     m_rego600;
    static constexpr const char* TAG = "Rego600Control";

public:
    rego600_number* heat_curve;
    rego600_number* heat_curve_fine_adjust;
    rego600_number* indoor_temperature_target;
    rego600_number* curve_influence_by_indoor_temperature;
    rego600_number* curve_adjust_at_plus_20;
    rego600_number* curve_adjust_at_plus_15;
    rego600_number* curve_adjust_at_plus_10;
    rego600_number* curve_adjust_at_plus_05;
    rego600_number* curve_adjust_at_plus_00;
    rego600_number* curve_adjust_at_minus_05;
    rego600_number* curve_adjust_at_minus_10;
    rego600_number* curve_adjust_at_minus_15;
    rego600_number* curve_adjust_at_minus_20;
    rego600_number* curve_adjust_at_minus_25;
    rego600_number* curve_adjust_at_minus_30;
    rego600_number* curve_adjust_at_minus_35;
    rego600_number* heat_curve_coupling;

    rego600_control(rego600* rego)
        : PollingComponent(CONTROL_REGISTER_INTERVAL)
        , m_rego600(rego)
        , heat_curve(new rego600_number("Heat curve", rego, HEAT_CURVE, 10.0f, 0.0f))
        , heat_curve_fine_adjust(new rego600_number("Heat curve fine adjust", rego, HEAT_CURVE_FINE_ADJUST, 10.0f, -10.0f, "°C"))
        , indoor_temperature_target(new rego600_number("Indoor temperature target", rego, INDOOR_TEMPERATURE_TARGET, 30.0f, 10.0f, "°C"))
        , curve_influence_by_indoor_temperature(
              new rego600_number("Curve influence by indoor temperature", rego, CURVE_INFLUENCE_BY_INDOOR_TEMPERATURE, 10.0f, -10.0f))
        , curve_adjust_at_plus_20(new rego600_number("Curve adjust at +20°C", rego, CURVE_ADJUST_AT_PLUS_20, 10.0f, -10.0f))
        , curve_adjust_at_plus_15(new rego600_number("Curve adjust at +15°C", rego, CURVE_ADJUST_AT_PLUS_15, 10.0f, -10.0f))
        , curve_adjust_at_plus_10(new rego600_number("Curve adjust at +10°C", rego, CURVE_ADJUST_AT_PLUS_10, 10.0f, -10.0f))
        , curve_adjust_at_plus_05(new rego600_number("Curve adjust at +5°C", rego, CURVE_ADJUST_AT_PLUS_05, 10.0f, -10.0f))
        , curve_adjust_at_plus_00(new rego600_number("Curve adjust at +0°C", rego, CURVE_ADJUST_AT_PLUS_00, 10.0f, -10.0f))
        , curve_adjust_at_minus_05(new rego600_number("Curve adjust at -5°C", rego, CURVE_ADJUST_AT_MINUS_05, 10.0f, -10.0f))
        , curve_adjust_at_minus_10(new rego600_number("Curve adjust at -10°C", rego, CURVE_ADJUST_AT_MINUS_10, 10.0f, -10.0f))
        , curve_adjust_at_minus_15(new rego600_number("Curve adjust at -15°C", rego, CURVE_ADJUST_AT_MINUS_15, 10.0f, -10.0f))
        , curve_adjust_at_minus_20(new rego600_number("Curve adjust at -20°C", rego, CURVE_ADJUST_AT_MINUS_20, 10.0f, -10.0f))
        , curve_adjust_at_minus_25(new rego600_number("Curve adjust at -25°C", rego, CURVE_ADJUST_AT_MINUS_25, 10.0f, -10.0f))
        , curve_adjust_at_minus_30(new rego600_number("Curve adjust at -30°C", rego, CURVE_ADJUST_AT_MINUS_30, 10.0f, -10.0f))
        , curve_adjust_at_minus_35(new rego600_number("Curve adjust at -35°C", rego, CURVE_ADJUST_AT_MINUS_35, 10.0f, -10.0f))
        , heat_curve_coupling(new rego600_number("Heat curve coupling difference", rego, HEAT_CURVE_COUPLING, 15.0f, 0.0f))

    {
    }

    void update();
};

#define get_control(identifier) static_cast<rego600_control*>(id(identifier).get_component(0))
#endif
