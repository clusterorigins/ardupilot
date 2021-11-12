#include <AP_BoardConfig/AP_BoardConfig.h>
//#include <AP_Logger/AP_Logger.h>
#include <AP_Vehicle/AP_Vehicle_Type.h>

extern const AP_HAL::HAL &hal;

???const AP_Param::GroupInfo RangeFinder::var_info[] = {???
???add backend???   
    
MagTest::MagTest()
{
    AP_Param::setup_object_defaults(this, var_info);
}

void MagTest::update(void)
{
    for (uint8_t i=0; i<num_instances; i++) {
        if (drivers[i] != nullptr) {
            if ((Type)params[i].type.get() == Type::NONE) {
                // allow user to disable a rangefinder at runtime
                state[i].status = Status::NotConnected;
                state[i].range_valid_count = 0;
                continue;
            }
            drivers[i]->update();
        }
    }
}
