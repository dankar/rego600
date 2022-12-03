#include "rego600_buttons.h"
#include "rego600_definitions.h"

bool rego600_buttons::press_power() { return m_rego600->press_button(POWER); }
bool rego600_buttons::press_key1() { return m_rego600->press_button(KEY_1); }
bool rego600_buttons::press_key2() { return m_rego600->press_button(KEY_2); }
bool rego600_buttons::press_key3() { return m_rego600->press_button(KEY_3); }
bool rego600_buttons::press_wheel_left() { return m_rego600->press_button(WHEEL_LEFT); }
bool rego600_buttons::press_wheel_right() { return m_rego600->press_button(WHEEL_RIGHT); }
