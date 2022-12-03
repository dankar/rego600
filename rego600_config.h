#ifndef _REGO600_CONFIG_H_
#define _REGO600_CONFIG_H_

// These values specify the interval between reading different
// categories of registers. Some are used for monitoring, resulting
// in a shorter update time, while some are control values and will
// only change on user action.
//
// Intervals are mostly prime numbers, just to reduce risk of us
// trying to read everything at once (which would not be a problem anyway)
//
// All values are in milliseconds

// Once an hour since these will only change if we change them, and
// if we change them via the protocol, we will read the updated value
// immediately
#define CONTROL_REGISTER_INTERVAL (60 * 60000)

// Device (compressor, heater cartridge etc.) on/off values
#define STATUS_REGISTER_INTERVAL (23000)

// Target temperature registers
#define TARGET_REGISTER_INTERVAL (29000)

// Temperature sensor registers, outdoor temp, fluid temps etc.
#define TEMPERATURE_REGISTER_INTERVAL (19000)

#endif
