#include "rego600_status.h"

using namespace esphome;

void rego600_status::update()
{
    for (auto el : m_status_map) {
        std::optional<int16_t> result;
        result = m_rego600->read_status(std::get<0>(el));

        if (result.has_value()) {
            std::get<1>(el)->publish_state(result.value() ? true : false);
        }
    }

    for (auto el : m_light_map) {
        std::optional<bool> result;
        result = m_rego600->read_panel_light(std::get<0>(el));

        if (result.has_value()) {
            std::get<1>(el)->publish_state(result.value());
        }
    }
}
