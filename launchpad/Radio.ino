#include "rocket.h"
#include "RocketModule.h"
///https://github.com/greiman/SdFat/archive/master.zip
#include "rocket.h"
namespace Radio {
    //configured for the razor imu
    const int bytesBeforeDelay = 16;
    int bytesWritten = 0;
    class RadioModule: public Rocket::RocketModule {
    public:
        virtual bool warmup() {
            //it's all RX TX, we can only hope
            Serial1.begin(9600);
            while(!Serial1);
            return true;
        }
        virtual void refresh() {
            Serial1.write(Rocket::DATA_START, Rocket::DATA_LEN);
            bytesWritten += Rocket::DATA_LEN;
            if(bytesWritten > bytesBeforeDelay) {
                delay(1);
            }
        }
    };
    RadioModule module;
    Rocket::RocketModule *handler = &module;
}