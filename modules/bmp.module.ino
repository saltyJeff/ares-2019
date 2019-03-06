///https://github.com/adafruit/Adafruit_BMP3XX/archive/master.zip
///https://github.com/adafruit/Adafruit_Sensor/archive/master.zip
#include "RocketModule.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <FlashStorage.h>
namespace BMP {
    extern float altitude;

    FlashStorage(seaLevelHolder, float);

    const int MISO = 8;
    const int MOSI = 9;
    const int SCK = 10;
    const int BMP_CS = 11;

    Adafruit_BMP3XX bmp(BMP_CS, MOSI, MISO, SCK);
    float seaLevel = seaLevelHolder.read();
    class BmpModule: public Rocket::RocketModule {
    public:
        virtual void preWarmup() {
            pinMode(MISO, OUTPUT);
            pinMode(MOSI, OUTPUT);
            pinMode(SCK, OUTPUT);
            pinMode(BMP_CS, OUTPUT);
            digitalWrite(BMP_CS, HIGH);
        }
        virtual bool warmup() {
            bool setupRight = bmp.begin();
            if(!setupRight) {
                return false;
            }
            bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
            bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
            return setupRight;
        }
        virtual void calibrate() {
            SerialUSB.println(F("Enter the pressure in hPAs at sea level"));
            seaLevelHolder.write(SerialUSB.parseFloat());
            seaLevel = seaLevelHolder.read();
            SerialUSB.read(); //discard newline
        }
        virtual void refresh() {
            if(!bmp.performReading()) {
                return;
            }
            altitude = bmp.readAltitude(seaLevel);
        }
    };
    BmpModule module;
    Rocket::RocketModule *handler = &module;
}