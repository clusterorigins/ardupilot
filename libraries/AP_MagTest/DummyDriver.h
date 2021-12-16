#pragma once

#include "AP_MagTest.h"
#include "AP_MagTest_Backend_Serial.h"

class DummyDriver : public AP_MagTest_Backend_Serial
{

public:

    using AP_MagTest_Backend_Serial::AP_MagTest_Backend_Serial;

protected:

    MAV_DISTANCE_SENSOR _get_mav_distance_sensor_type() const override {
        return MAV_DISTANCE_SENSOR_LASER;
    }

private:
    // get a reading
    bool get_reading(uint16_t &reading_cm) override;

    char linebuf[10];           // legacy protocol buffer
    uint8_t linebuf_len;        // legacy protocol buffer length
    uint32_t last_init_ms;      // init time used to switch lw20 to serial mode
    uint8_t high_byte;          // binary protocol high byte
    bool high_byte_received;    // true if high byte has been received

    // automatic protocol decision variables
    enum class ProtocolState {
        UNKNOWN,    // the protocol used is not yet known
        LEGACY,     // legacy protocol, distances are sent as strings
        BINARY      // binary protocol, distances are sent using two bytes
    } protocol_state;
    uint8_t legacy_valid_count;
    uint8_t binary_valid_count;
};
