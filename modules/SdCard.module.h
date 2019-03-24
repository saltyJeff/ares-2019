#include <SdFat.h>

#define SD_CHIPSEL 38

namespace SdCard {
    class Handler: public Rocket::RocketModule {
    public:
        SdFat sdCard;
        SdBaseFile saveFile;
        int bytesWritten = 0;
        const char *fileName = "rocket_data.bin";
        virtual bool warmup() {
            SerialUSB.println(sdCard.card()->errorCode());
            if(!sdCard.begin(SD_CHIPSEL)) {
                if(sdCard.card()->errorCode()) {
                    SerialUSB.print("Error code: ");
                    SerialUSB.println(sdCard.card()->errorCode(), HEX);
                    SerialUSB.println(sdCard.card()->errorData());
                }
                else {
                    SerialUSB.println("need a reformat");
                }
                return false;
            }
            SerialUSB.print("Trying to create file: ");
            if(saveFile.open(fileName, O_CREAT | O_WRITE | O_TRUNC)) {
                SerialUSB.println("Created file");
                return true;
            }
            else {
                SerialUSB.println("Couldn't create the file");
                return false;
            }
            SerialUSB.println("all good");
            return true;
        }
        virtual void refresh() {
            //Remainder will store how many bytes overflow
            //Assume max size is 8 bytes, data len is 3, bytes written is 7
            int remainder = bytesWritten + Rocket::DATA_LEN - 512; //2
            remainder = max(0, remainder); //2
            int k = saveFile.write(Rocket::DATA_START, Rocket::DATA_LEN - remainder); //writes [data+0, 1)
            if(bytesWritten + Rocket::DATA_LEN >= 512) { //if 7 + 3 >= 8 (true)
                int startMillis = millis();
                //SerialUSB.println("Write buffer flushed");
                saveFile.close(); //flush
                saveFile.open(fileName, O_WRITE | O_APPEND);
                bytesWritten = remainder;
                if(bytesWritten > 0) { //true
                    k += saveFile.write(Rocket::DATA_START + Rocket::DATA_LEN - remainder, remainder); //writes [data+2, 6)
                } //4
                //SerialUSB.print("Write flushed: ");
                //SerialUSB.println(millis() - startMillis);
            }
            else {
                bytesWritten += Rocket::DATA_LEN;
            }
            //SerialUSB.println(bytesWritten);
        }
        virtual void shutdown() {
            saveFile.close();
        }
    };
}