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
    const int bytesBeforeFlush = 512;
    
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
                SerialUSB.println(F("Failed to initialize card"));
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
            //Remainder will store how many bytes overflow
            //Assume max size is 8 bytes, data len is 6, bytes written is 6
            int remainder = bytesWritten + ROCKET::DATA_LEN - 512; //4
            remainder = max(0, remainder); //4
            file.write(ROCKET::DATA_START, ROCKET::DATA_LEN - remainder); //writes [data+0, 2)
            if(bytesWritten + ROCKET::DATA_LEN >= 512) { //if 6 + 6 > 8 (true)
                file.flush(); //flush
                bytesWritten = remainder; //4
            }
            if(bytesWritten > 0) { //true
                file.write(ROCKET::DATA_START + ROCKET::DATA_LEN - remainder, remainder); //writes [data+2, 6)
            }
        }
        virtual void shutdown() {
            file.flush();
        }
    };
    SdModule module;
    Rocket::RocketModule *handler = &module;
}