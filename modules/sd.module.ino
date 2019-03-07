///https://github.com/greiman/SdFat/archive/master.zip
#include <SPI.h>
#include <SdFat.h>
#include "rocket.h"
namespace SdCard {
    #ifdef __avr__
    const int SD_CHIP_SELECT_PIN = 9;
    #else
    //configured for the razor imu
    const int SD_CHIP_SELECT_PIN = 38;
    #endif
    const int bytesBeforeFlush = 400;
    
    SdFat SDCard;
    SdFile file;
    int bytesWritten = 0;
    class SdModule: public Rocket::RocketModule {
    public:
        virtual void preWarmup() {
            pinMode(SD_CHIP_SELECT_PIN, OUTPUT);
            digitalWrite(SD_CHIP_SELECT_PIN, HIGH);
        }
        virtual bool warmup() {
            bool worked = SDCard.begin(SD_CHIP_SELECT_PIN, SPI_FULL_SPEED);
            if(!worked) {
                SerialUSB.println("Failed to initialize card");
                return worked;
            }
            worked = file.open("rocket_log.bin", O_TRUNC | O_CREAT | O_WRITE);
            if(!worked) {
                SerialUSB.println(F("Failed to initialize file"));
                return worked;
            }
            return worked;
        }
        virtual void refresh() {
            file.write(Rocket::DATA_START, Rocket::DATA_LEN);
            bytesWritten += Rocket::DATA_LEN;
            if(bytesWritten > bytesBeforeFlush) {
                file.flush();
                bytesWritten = 0;
            }
        }
        virtual void shutdown() {
            file.flush();
        }
    };
    SdModule module;
    Rocket::RocketModule *handler = &module;
}