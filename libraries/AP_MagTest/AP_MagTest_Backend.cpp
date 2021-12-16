#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include "AP_MagTest.h"
#include "AP_MagTest_Backend.h"

extern const AP_HAL::HAL& hal;

/*
  base class constructor. 
  This incorporates initialisation as well.
*/
AP_MagTest_Backend::AP_MagTest_Backend(MagTest::MagTest_State &_state, AP_MagTest_Params &_params) :
        state(_state),
		params(_params)
{
    _backend_type = type();
}

MAV_DISTANCE_SENSOR AP_MagTest_Backend::get_mav_distance_sensor_type() const {
    if (type() == MagTest::Type::NONE) {
        return MAV_DISTANCE_SENSOR_UNKNOWN;
    }
    return _get_mav_distance_sensor_type();
}

MagTest::Status AP_MagTest_Backend::status() const {
    if (type() == MagTest::Type::NONE) {
        // turned off at runtime?
        return MagTest::Status::NotConnected;
    }
    return state.status;
}

// true if sensor is returning data
bool AP_MagTest_Backend::has_data() const {
    return ((state.status != MagTest::Status::NotConnected) &&
            (state.status != MagTest::Status::NoData));
}

// update status based on distance measurement
void AP_MagTest_Backend::update_status()
{
    // check distance
    if ((int16_t)state.distance_cm > max_distance_cm()) {
        set_status(MagTest::Status::OutOfRangeHigh);
    } else if ((int16_t)state.distance_cm < min_distance_cm()) {
        set_status(MagTest::Status::OutOfRangeLow);
    } else {
        set_status(MagTest::Status::Good);
    }
}

// set status and update valid count
void AP_MagTest_Backend::set_status(MagTest::Status _status)
{
    state.status = _status;

    // update valid count
    if (_status == MagTest::Status::Good) {
        if (state.range_valid_count < 10) {
            state.range_valid_count++;
        }
    } else {
        state.range_valid_count = 0;
    }
}
