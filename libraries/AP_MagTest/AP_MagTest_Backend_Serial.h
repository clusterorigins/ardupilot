#pragma once

#include "AP_MagTest_Backend.h"

class AP_MagTest_Backend_Serial : public AP_MagTest_Backend
{
public:
    // constructor
    AP_MagTest_Backend_Serial(MagTest::MagTest_State &_state,
                                  AP_MagTest_Params &_params,
                                  uint8_t serial_instance);

    // static detection function
    static bool detect(uint8_t serial_instance);

protected:

    // baudrate used during object construction:
    virtual uint32_t initial_baudrate(uint8_t serial_instance) const;

    // the value 0 is special to the UARTDriver - it's "use default"
    virtual uint16_t rx_bufsize() const { return 0; }
    virtual uint16_t tx_bufsize() const { return 0; }

    AP_HAL::UARTDriver *uart = nullptr;

    // update state; not all backends call this!
    virtual void update(void) override;

    // it is essential that anyone relying on the base-class update to
    // implement this:
    virtual bool get_reading(uint16_t &reading_cm) = 0;

    // maximum time between readings before we change state to NoData:
    virtual uint16_t read_timeout_ms() const { return 200; }
};
