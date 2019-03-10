#include "rocket.h"
#include "RocketModule.h"
///https://github.com/adafruit/Adafruit_BMP3XX/archive/master.zip
///https://github.com/adafruit/Adafruit_Sensor/archive/master.zip
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

namespace BMP {
    float &altitude = Rocket::data.BMP_altitude;
    float &temperature = Rocket::data.BMP_temperature;
    float &pressure = Rocket::data.BMP_pressure;

    float seaLevel = 1013.25;

    const int MISO = 8;
    const int MOSI = 9;
    const int SCK = 10;
    #ifdef __AVR__
    const int BMP_CS = 10;
    Adafruit_BMP3XX bmp(BMP_CS);
    #else
    const int BMP_CS = 11;
    Adafruit_BMP3XX bmp(BMP_CS, MOSI, MISO, SCK);
    #endif
    class BmpModule: public Rocket::RocketModule {
    public:
        virtual void preWarmup() {
            #ifndef __AVR__
            pinMode(MISO, OUTPUT);
            pinMode(MOSI, OUTPUT);
            pinMode(SCK, OUTPUT);
            pinMode(BMP_CS, OUTPUT);
            #endif
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
        virtual void refresh() {
            if(!bmp.performReading()) {
                return;
            }
            altitude = bmp.readAltitude(seaLevel);
            temperature = bmp.readTemperature();
            pressure = bmp.readPressure();
        }
    };
    BmpModule module;
    Rocket::RocketModule *handler = &module;
}