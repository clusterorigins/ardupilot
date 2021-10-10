#include <ctype.h>

extern const AP_HAL::HAL& hal;


AP_MagTest_Backend_Serial::AP_RangeFinder_Backend_Serial(
    MagTest::MagTest_State &_state,
    AP_MagTest_Params &_params,
    uint8_t serial_instance) :
    AP_MagTest_Backend(_state, _params)
{
    uart = AP::serialmanager().find_serial(AP_SerialManager::SerialProtocol_Rangefinder, serial_instance);
    if (uart != nullptr) {
        uart->begin(initial_baudrate(serial_instance), rx_bufsize(), tx_bufsize());
    }
}
void AP_MagTest_Backend_Serial::update(void)
{
    if (get_reading(state.distance_cm)) {
       
        state.last_reading_ms = AP_HAL::millis();
        update_status();
    } else if (AP_HAL::millis() - state.last_reading_ms > read_timeout_ms()) {
        set_status(MagTest::Status::NoData);
    }
}
