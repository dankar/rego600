#ifndef _REGO600_DISPLAY_H_
#define _REGO600_DISPLAY_H_

#include "esphome.h"
#include "rego600.h"
#include "rego600_config.h"
#include "rego600_definitions.h"
#include <tuple>
#include <vector>

class rego600_display : public esphome::PollingComponent {
private:
    rego600*                     m_rego600;
    static constexpr const char* TAG = "Rego600Display";

public:
    esphome::text_sensor::TextSensor* display_line1 = new esphome::text_sensor::TextSensor();
    esphome::text_sensor::TextSensor* display_line2 = new esphome::text_sensor::TextSensor();
    esphome::text_sensor::TextSensor* display_line3 = new esphome::text_sensor::TextSensor();
    esphome::text_sensor::TextSensor* display_line4 = new esphome::text_sensor::TextSensor();

    rego600_display(rego600* rego)
        : PollingComponent(DISPLAY_READ_INTERVAL)
        , m_rego600(rego)
    {
    }

    void update() override final;
};

#endif
