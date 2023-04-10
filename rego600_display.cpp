#include "rego600_display.h"

using namespace esphome;

std::string latin9_to_utf8(const std::string input)
{
    std::string result;
    size_t      n = 0;

    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] < 128) {
            result.push_back(input[i]);
        } else if (input[i] < 192)
            switch (input[i]) {
            case 164:
                result.push_back(226);
                result.push_back(130);
                result.push_back(172);
                break;
            case 166:
                result.push_back(197);
                result.push_back(160);
                break;
            case 168:
                result.push_back(197);
                result.push_back(161);
                break;
            case 180:
                result.push_back(197);
                result.push_back(189);
                break;
            case 184:
                result.push_back(197);
                result.push_back(190);
                break;
            case 188:
                result.push_back(197);
                result.push_back(146);
                break;
            case 189:
                result.push_back(197);
                result.push_back(147);
                break;
            case 190:
                result.push_back(197);
                result.push_back(184);
                break;
            default:
                result.push_back(194);
                result.push_back(input[i]);
                break;
            }
        else {
            result.push_back(195);
            result.push_back(input[i] - 64);
        }
    }

    return result;
}

void rego600_display::update()
{
    auto disp = m_rego600->read_display();

    if (disp.has_value()) {
        display_line1->publish_state(latin9_to_utf8(disp.value()[0]));
        display_line2->publish_state(latin9_to_utf8(disp.value()[1]));
        display_line3->publish_state(latin9_to_utf8(disp.value()[2]));
        display_line4->publish_state(latin9_to_utf8(disp.value()[3]));
    }
}
