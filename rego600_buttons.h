#ifndef _REGO600_BUTTONS_H_
#define _REGO600_BUTTONS_H_

#include "esphome.h"
#include "rego600.h"

class rego600_buttons : public esphome::Component {
private:
    rego600*                     m_rego600;
    static constexpr const char* TAG = "Rego600Buttons";

public:
    rego600_buttons(rego600* rego)
        : m_rego600(rego)
    {
    }

    bool press_power();
    bool press_key1();
    bool press_key2();
    bool press_key3();
    bool press_wheel_left();
    bool press_wheel_right();
};

#define get_buttons(identifier) static_cast<rego600_buttons*>(id(identifier).get_component(0))

#endif
