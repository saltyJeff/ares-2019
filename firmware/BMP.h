#pragma once
#include "RocketModule.h"
///https://github.com/adafruit/Adafruit_BMP3XX/archive/master.zip
///https://github.com/adafruit/Adafruit_Sensor/archive/master.zip
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

namespace Bmp {
    float &altitude = Rocket::data.Bmp_altitude;
    float &temperature = Rocket::data.Bmp_temperature;
    float &pressure = Rocket::data.Bmp_pressure;

    float seaLevel = 1013.25;

    const int MISO = 8;
    const int MOSI = 9;
    const int SCK = 10;
    const int BMP_CS = 11;
    Adafruit_BMP3XX bmp(BMP_CS, MOSI, MISO, SCK);

    class Handler: public Rocket::RocketModule {
    public:
        virtual void preWarmup() {
            digitalWrite(BMP_CS, HIGH);
        }
        virtual bool warmup() {
            bool setupRight = bmp.begin();
            if(!setupRight) {
                return false;
            }
            bmp.setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
            bmp.setPressureOversampling(BMP3_NO_OVERSAMPLING);
            //bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
            bmp.setOutputDataRate(BMP3_ODR_200_HZ);
            return setupRight;
        }
        virtual void refresh() {
            if(!bmp.performReading()) {
                return;
            }
            //each read will cache the values for a given reading, no need
            //to perform another reading for temp/press
            altitude = bmp.readAltitude(seaLevel);
            temperature = bmp.temperature;
            pressure = bmp.pressure;
        }
    };
}