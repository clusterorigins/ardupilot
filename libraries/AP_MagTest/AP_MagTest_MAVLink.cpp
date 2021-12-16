#include "AP_MagTest_MAVLink.h"
#include <AP_HAL/AP_HAL.h>

/*
   Set the distance based on a MAVLINK message
*/
void AP_MagTest_MAVLink::handle_msg(const mavlink_message_t &msg)
{
    mavlink_distance_sensor_t packet;
    mavlink_msg_distance_sensor_decode(&msg, &packet);

    // only accept distances for downward facing sensors
    if (packet.orientation == MAV_SENSOR_ROTATION_PITCH_270) {
        state.last_reading_ms = AP_HAL::millis();
        distance_cm = packet.current_distance;
        _max_distance_cm = packet.max_distance;
        _min_distance_cm = packet.min_distance;
        sensor_type = (MAV_DISTANCE_SENSOR)packet.type;
    }
}

int16_t AP_MagTest_MAVLink::max_distance_cm() const
{
    if (_max_distance_cm == 0 && _min_distance_cm == 0) {
        // we assume if both of these are zero that we ignore both
        return params.max_distance_cm;
    }

    if (params.max_distance_cm < _max_distance_cm) {
        return params.max_distance_cm;
    }
    return _max_distance_cm;
}
int16_t AP_MagTest_MAVLink::min_distance_cm() const
{
    if (_max_distance_cm == 0 && _min_distance_cm == 0) {
        // we assume if both of these are zero that we ignore both
        return params.min_distance_cm;
    }
    if (params.min_distance_cm > _min_distance_cm) {
        return params.min_distance_cm;
    }
    return _min_distance_cm;
}

/*
   update the state of the sensor
*/
void AP_MagTest_MAVLink::update(void)
{
    //Time out on incoming data; if we don't get new
    //data in 500ms, dump it
    if (AP_HAL::millis() - state.last_reading_ms > AP_RANGEFINDER_MAVLINK_TIMEOUT_MS) {
        set_status(MagTest::Status::NoData);
        state.distance_cm = 0;
    } else {
        state.distance_cm = distance_cm;
        update_status();
    }
}
