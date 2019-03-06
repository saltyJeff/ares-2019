///https://github.com/greiman/SdFat/archive/master.zip
#include <SPI.h>
#include <SdFat.h>
#include <FlashStorage.h>
#include "rocket.h"
namespace SdCard {
    //configured for the razor imu
    const int SD_CHIP_SELECT_PIN = 38;
    const int bytesBeforeFlush = 400;
    FlashStorage(fileNo, int);
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
            int fileNum = fileNo.read();
            fileNum++;
            String fileName = String("rocket_log");
            fileName += String(fileNum);
            fileName += String(".bin");
            worked = file.open(fileName.c_str(), O_TRUNC | O_CREAT | O_WRITE);
            if(!worked) {
                SerialUSB.println(F("Failed to initialize file"));
                return worked;
            }
            fileNo.write(fileNum);
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