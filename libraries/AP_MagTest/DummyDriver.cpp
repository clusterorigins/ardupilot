#include <AP_HAL/AP_HAL.h>
#include <ctype.h>
#include "DummyDriver.h"
extern const AP_HAL::HAL& hal;



// read - return last value measured by sensor
bool DummyDriver::get_reading(uint16_t &reading_cm)
{
    reading_cm = 5;
    return true;
}
