#ifndef _REGO600_DEFINITIONS_H_
#define _REGO600_DEFINITIONS_H_

struct __attribute__((packed)) command_t {
    uint8_t target;
    uint8_t operation;
    uint8_t reg[3];
    uint8_t value[3];
    uint8_t checksum;
};

enum target_t { REGO_TARGET = 0x81, PC_TARGET = 0x01 };

enum operation_t {
    READ_FRONT_PANEL      = 0x0000,
    WRITE_FRONT_PANEL     = 0x0001,
    READ_SYSTEM_REGISTER  = 0x0002,
    WRITE_SYSTEM_REGISTER = 0x0003,
    READ_TIMER_REGISTER   = 0x0004,
    WRITE_TIMER_REGISTER  = 0x0005,
    READ_REGISTER_1B61    = 0x0006,
    WRITE_REGISTER_1B61   = 0x0007,
    READ_DISPLAY          = 0x0020,
    READ_LAST_ERROR       = 0x0040,
    READ_PREVIOUS_ERROR   = 0x0042,
    READ_VERSION          = 0x007f
};

enum front_panel_button_t {
    POWER = 0x0008,
    KEY_1 = 0x0009,
    KEY_2 = 0x000A,
    KEY_3 = 0x000B,
    WHEEL = 0x0044,
    // Special wheel values
    WHEEL_LEFT  = 0xff00,
    WHEEL_RIGHT = 0xff01
};

enum front_panel_light_t {
    LED_POWER    = 0x0012,
    LED_PUMP     = 0x0013,
    LED_ADD_HEAT = 0x0014,
    LED_BOILER   = 0x0015,
    LED_ALARM    = 0x0016,
};

enum wheel_value_t { WHEEL_LEFT_VALUE = 0xFFFF, WHEEL_RIGHT_VALUE = 0x0001 };

enum sensor_register_t {
    RADIATOR_RETURN_TEMPERATURE    = 0x0209,
    OUTDOOR_TEMPERATURE            = 0x020A,
    WARM_WATER_TEMPERATURE         = 0x020B,
    RADIATOR_FORWARD_TEMPERATURE   = 0x020C,
    ROOM_TEMPERATURE               = 0x020D,
    COMPRESSOR_TEMPERATURE         = 0x020E,
    HEAT_FLUID_OUT_TEMPERATURE     = 0x020F,
    HEAT_FLUID_IN_TEMPERATURE      = 0x0210,
    COLD_FLUID_IN_TEMPERATURE      = 0x0211,
    COLD_FLUID_OUT_TEMPERATURE     = 0x0212,
    EXTERNAL_HOT_WATER_TEMPERATURE = 0x0213,

    RADIATOR_RETURN_TARGET_TEMPERATURE = 0x006e,
    RADIATOR_RETURN_ON_TEMPERATURE     = 0x006f,
    RADIATOR_RETURN_OFF_TEMPERATURE    = 0x0070,
    WARM_WATER_TARGET_TEMPERATURE      = 0x002b,
    WARM_WATER_ON_TEMPERATURE          = 0x0073,
    WARM_WATER_OFF_TEMPERATURE         = 0x0074,
    FORWARD_TARGET_TEMPERATURE         = 0x006d,
    ADD_HEAT_PERCENTAGE                = 0x006c
};

enum status_register_t {
    // device values
    GROUND_LOOP_PUMP   = 0x01fd,
    COMPRESSOR         = 0x01fe,
    HEAT_CARTRIDGE_3KW = 0x01ff,
    HEAT_CARTRIDGE_6KW = 0x0200,
    RADIATOR_PUMP      = 0x0203,
    HEAT_CARRIER_PUMP  = 0x0204,
    THREE_WAY_VALVE    = 0x0205,
    ALARM              = 0x0206
};

enum control_register_t {
    // settings
    HEAT_CURVE                            = 0x0000,
    HEAT_CURVE_FINE_ADJUST                = 0x0001,
    INDOOR_TEMPERATURE_TARGET             = 0x0021,
    CURVE_INFLUENCE_BY_INDOOR_TEMPERATURE = 0x0022,
    CURVE_ADJUST_AT_PLUS_20               = 0x001E,
    CURVE_ADJUST_AT_PLUS_15               = 0x001C,
    CURVE_ADJUST_AT_PLUS_10               = 0x001A,
    CURVE_ADJUST_AT_PLUS_05               = 0x0018,
    CURVE_ADJUST_AT_PLUS_00               = 0x0016,
    CURVE_ADJUST_AT_MINUS_05              = 0x0014,
    CURVE_ADJUST_AT_MINUS_10              = 0x0012,
    CURVE_ADJUST_AT_MINUS_15              = 0x0010,
    CURVE_ADJUST_AT_MINUS_20              = 0x000E,
    CURVE_ADJUST_AT_MINUS_25              = 0x000C,
    CURVE_ADJUST_AT_MINUS_30              = 0x000A,
    CURVE_ADJUST_AT_MINUS_35              = 0x0008,
    HEAT_CURVE_COUPLING                   = 0x0002
};

#endif
