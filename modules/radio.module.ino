///https://github.com/greiman/SdFat/archive/master.zip
#include "RocketModule.h"
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
            Serial1.write(Rocket::startDelim, 2);
            Serial1.write(Rocket::DATA_START, Rocket::DATA_LEN);
            Serial1.write(Rocket::endDelim, 2);
            bytesWritten += Rocket::DATA_LEN + 4;
            if(bytesWritten > bytesBeforeDelay) {
                delay(1);
            }
        }
    };
    RadioModule module;
    Rocket::RocketModule *handler = &module;
}