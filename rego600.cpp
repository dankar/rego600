#include "rego600.h"
#include "rego600_definitions.h"

using namespace esphome;
using namespace esphome::uart;

#ifndef millis
#define millis(x) esphome::millis(x)
#endif

std::optional<std::vector<uint8_t>> rego600::read_serial(size_t len)
{
    size_t   pos   = 0;
    uint32_t start = millis();

    std::vector<uint8_t> result;

    while (pos < len) {
        if (millis() - start > m_timeout) {
            return std::nullopt;
        }

        App.feed_wdt();

        if (m_uart.available()) {
            uint8_t c;
            if (m_uart.read_byte(&c)) {
                result.push_back(c);
                pos++;
            }
        }
    }

    return result;
}

void rego600::flush_receive()
{
    while (m_uart.available()) {
        uint8_t c;
        m_uart.read_byte(&c);
    }
}

bool rego600::write_serial(const std::vector<uint8_t>& data)
{
    m_uart.write_array(data);
    return true;
}

int16_t rego600::read_integer(const uint8_t* data)
{
    int16_t res = ((int16_t) * (data) << 14) | ((int16_t) * (data + 1) << 7) | (int16_t) * (data + 2);
    return res;
}

float rego600::integer_to_float(int16_t i)
{
    float temp = 0.1 * (float)i;
    return temp;
}

int16_t rego600::float_to_integer(float f)
{
    int16_t temp = (int16_t)(f * 10);
    return temp;
}

void rego600::write_integer(int16_t val, uint8_t* data)
{
    *(data + 2) = (uint8_t)val & 0x7F;
    *(data + 1) = (uint8_t)(val >> 7) & 0x7F;
    *data       = (uint8_t)(val >> 14) & 0x03;
}

uint8_t rego600::calculate_checksum(const uint8_t* data, size_t len)
{
    uint8_t result = 0;
    for (size_t i = 0; i < len; i++) {
        result ^= data[i];
    }
    return result;
}

bool rego600::read_ack_response()
{
    auto response = read_serial(1);

    if (!response) {
        ESP_LOGE(TAG, "Timeout waiting for response");
        return false;
    }

    auto c = response.value()[0];

    if (c != PC_TARGET) {
        ESP_LOGE(TAG, "Unexpected response, expected 0x01, got 0x%02x", c);
        return false;
    }

    return true;
}

std::optional<int16_t> rego600::read_response()
{
    auto response = read_serial(5);

    if (!response) {
        ESP_LOGE(TAG, "Timeout waiting for response");
        return std::nullopt;
    }

    auto resp = response.value();

    if (resp[0] != PC_TARGET) {
        ESP_LOGE(TAG, "Wrong target for response, got 0x%02x", resp[0]);
        return std::nullopt;
    }

    if (calculate_checksum(&resp[1], 4) != 0) {
        ESP_LOGE(TAG, "Incorrect checksum in response");
        return std::nullopt;
    }

    return read_integer(&resp[1]);
}

std::optional<int16_t> rego600::run_command(target_t target, operation_t operation, uint16_t reg, uint16_t value, response_type_t response)
{

    command_t              cmd;
    std::optional<int16_t> result;

    cmd.target    = target;
    cmd.operation = operation;
    write_integer(reg, cmd.reg);
    write_integer(value, cmd.value);
    cmd.checksum = calculate_checksum(cmd.reg, 6);

    std::vector<uint8_t> data(sizeof(command_t));

    memcpy(&data[0], &cmd, sizeof(command_t));

    flush_receive();

    if (write_serial(data)) {
        if (response == ONE_BYTE_RESPONSE) {
            if (read_ack_response()) {
                result = 1;
            }
        } else if (response == FIVE_BYTE_RESPONSE) {
            result = read_response();
        }
    }

    flush_receive();

    return result;
}

std::optional<float> rego600::read_sensor_register(sensor_register_t reg)
{
    auto response = run_command(REGO_TARGET, READ_SYSTEM_REGISTER, reg, 0, FIVE_BYTE_RESPONSE);

    if (!response) {
        return std::nullopt;
    }

    return integer_to_float(response.value());
}

bool rego600::write_button_register(front_panel_button_t reg, uint16_t val)
{
    return run_command(REGO_TARGET, WRITE_FRONT_PANEL, reg, val, ONE_BYTE_RESPONSE).has_value();
}

std::optional<bool> rego600::read_light_register(front_panel_light_t reg)
{
    auto response = run_command(REGO_TARGET, READ_FRONT_PANEL, reg, 0, FIVE_BYTE_RESPONSE);

    if (!response) {
        return std::nullopt;
    }

    if (response.value()) {
        return true;
    } else {
        return false;
    }
}

std::optional<float> rego600::read_control_register(control_register_t reg)
{
    auto response = run_command(REGO_TARGET, READ_SYSTEM_REGISTER, reg, 0, FIVE_BYTE_RESPONSE);

    if (!response) {
        return std::nullopt;
    }

    return integer_to_float(response.value());
}

bool rego600::write_control_register(control_register_t reg, float val)
{
    int16_t tmp = float_to_integer(val);
    return run_command(REGO_TARGET, WRITE_SYSTEM_REGISTER, reg, tmp, ONE_BYTE_RESPONSE).has_value();
}

bool rego600::press_button(front_panel_button_t button)
{
    front_panel_button_t reg;
    int16_t              val;
    if (button == WHEEL_LEFT) {
        reg = WHEEL;
        val = WHEEL_LEFT_VALUE;
    } else if (button == WHEEL_RIGHT) {
        reg = WHEEL;
        val = WHEEL_RIGHT_VALUE;
    } else {
        reg = button;
        val = 0x0001;
    }

    if (!write_button_register(reg, val)) {
        ESP_LOGE(TAG, "Failed to send command");
    }

    return true;
}

std::optional<bool> rego600::read_panel_light(front_panel_light_t reg)
{
    auto response = read_light_register(reg);

    if (!response) {
        return std::nullopt;
    } else {
        if (response.value()) {
            return true;
        } else {
            return false;
        }
    }
}

std::optional<int16_t> rego600::read_status(status_register_t reg)
{
    return run_command(REGO_TARGET, READ_SYSTEM_REGISTER, reg, 0, FIVE_BYTE_RESPONSE);
}

std::optional<float> rego600::read_decimal_sensor(sensor_register_t reg) { return read_sensor_register(reg); }

temperatures_t rego600::read_all_sensors()
{
    temperatures_t temps;
    temps.radiator_return  = read_decimal_sensor(RADIATOR_RETURN_TEMPERATURE);
    temps.outdoor          = read_decimal_sensor(OUTDOOR_TEMPERATURE);
    temps.indoor           = read_decimal_sensor(ROOM_TEMPERATURE);
    temps.warm_water       = read_decimal_sensor(WARM_WATER_TEMPERATURE);
    temps.radiator_forward = read_decimal_sensor(RADIATOR_FORWARD_TEMPERATURE);
    temps.heat_fluid_in    = read_decimal_sensor(HEAT_FLUID_IN_TEMPERATURE);
    temps.heat_fluid_out   = read_decimal_sensor(HEAT_FLUID_OUT_TEMPERATURE);
    temps.cold_fluid_in    = read_decimal_sensor(COLD_FLUID_IN_TEMPERATURE);
    temps.cold_fluid_out   = read_decimal_sensor(COLD_FLUID_OUT_TEMPERATURE);

    return temps;
}

targets_t rego600::read_all_targets()
{
    targets_t targets;

    targets.radiator_return_target = read_decimal_sensor(RADIATOR_RETURN_TARGET_TEMPERATURE);
    targets.radiator_return_on     = read_decimal_sensor(RADIATOR_RETURN_ON_TEMPERATURE);
    targets.radiator_return_off    = read_decimal_sensor(RADIATOR_RETURN_OFF_TEMPERATURE);
    targets.warm_water_target      = read_decimal_sensor(WARM_WATER_TARGET_TEMPERATURE);
    targets.warm_water_on          = read_decimal_sensor(WARM_WATER_ON_TEMPERATURE);
    targets.warm_water_off         = read_decimal_sensor(WARM_WATER_OFF_TEMPERATURE);
    targets.add_heat_percentage    = read_decimal_sensor(ADD_HEAT_PERCENTAGE);

    return targets;
}
