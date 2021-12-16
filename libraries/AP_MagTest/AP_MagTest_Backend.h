#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include "AP_MagTest.h"

class AP_MagTest_Backend
{
public:
    // constructor. This incorporates initialisation as well.
	AP_MagTest_Backend(MagTest::MagTest_State &_state, AP_MagTest_Params &_params);

    // we declare a virtual destructor so that RangeFinder drivers can
    // override with a custom destructor if need be
    virtual ~AP_MagTest_Backend(void) {}

    // update the state structure
    virtual void update() = 0;

    virtual void handle_msg(const mavlink_message_t &msg) { return; }
#if HAL_MSP_RANGEFINDER_ENABLED
    virtual void handle_msp(const MSP::msp_rangefinder_data_message_t &pkt) { return; }
#endif

    enum Rotation orientation() const { return (Rotation)params.orientation.get(); }
    uint16_t distance_cm() const { return state.distance_cm; }
    uint16_t voltage_mv() const { return state.voltage_mv; }
    virtual int16_t max_distance_cm() const { return params.max_distance_cm; }
    virtual int16_t min_distance_cm() const { return params.min_distance_cm; }
    int16_t ground_clearance_cm() const { return params.ground_clearance_cm; }
    MAV_DISTANCE_SENSOR get_mav_distance_sensor_type() const;
    MagTest::Status status() const;
    MagTest::Type type() const { return (RangeFinder::Type)params.type.get(); }

    // true if sensor is returning data
    bool has_data() const;

    // returns count of consecutive good readings
    uint8_t range_valid_count() const { return state.range_valid_count; }

    // return a 3D vector defining the position offset of the sensor
    // in metres relative to the body frame origin
    const Vector3f &get_pos_offset() const { return params.pos_offset; }

    // return system time of last successful read from the sensor
    uint32_t last_reading_ms() const { return state.last_reading_ms; }

    // get temperature reading in C.  returns true on success and populates temp argument
    virtual bool get_temp(float &temp) { return false; }

protected:

    // update status based on distance measurement
    void update_status();

    // set status and update valid_count
    void set_status(MagTest::Status status);

    MagTest::MagTest_State &state;
    AP_MagTest_Params &params;

    // semaphore for access to shared frontend data
    HAL_Semaphore _sem;

    //Type Backend initialised with
    MagTest::Type _backend_type;

    virtual MAV_DISTANCE_SENSOR _get_mav_distance_sensor_type() const = 0;
};
