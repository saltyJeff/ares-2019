#pragma once
#include "RocketModule.h"
#define AVG_100(V) \
    [&] { \
    float total = 0; \
    for(int i = 0; i < 100; i++) { \
        total+= analogRead(V); \
    } \
    return total / 100.0; \
}();

namespace Analog {
    uint16_t &CC = Rocket::data.Analog_CC;
    uint16_t &fuel = Rocket::data.Analog_fuel;
    uint16_t &OX = Rocket::data.Analog_OX;
    uint16_t &press = Rocket::data.Analog_press;

    struct CalibData {
        float offsets[4];
        float scales[4];
    };

    FlashStorage(calibData, CalibData);

    class Handler: public Rocket::RocketModule {
    public:
        float *offsetVals;
        float *scaleVals;
        CalibData calib;
        virtual bool warmup() {
            calib = calibData.read();
            offsetVals = calib.offsets;
            scaleVals = calib.scales;
            for(int i = 0; i < 4; i++) {
                if(scaleVals[i] == 0) {
                    scaleVals[i] = 1;
                }
            }
            return true;
        }
        virtual void refresh() {
            CC = (analogRead(A0) - offsetVals[0]) * scaleVals[0];
            fuel = (analogRead(A1) - offsetVals[1]) * scaleVals[1];
            OX = (analogRead(A2) - offsetVals[2]) * scaleVals[2];
            press = (analogRead(A3) - offsetVals[3]) * scaleVals[3];
        }
        float readSensor(int id) {
            SerialUSB.println("Reading analog sensors");
            float f = 0;
            if(id == 0) {
                f = AVG_100(A0)
            }
            else if(id == 1) {
                f = AVG_100(A1)
            }
            else if(id == 2) {
                f = AVG_100(A2)
            }
            else if(id == 3) {
                f = AVG_100(A3)
            }
            SerialUSB.println(f);
            return f;
        }
        virtual void calibrate() {
            SerialUSB.println("Calibrating the Analog pins. Press enter when you're done with the instructions");            
            const char *sensorOrder[] = {
                "CC",
                "fuel",
                "OX",
                "press"
            };
            for(int i = 0; i < 4; i++) {
                SerialUSB.print("Null ");
                SerialUSB.println(sensorOrder[i]);
                
                while(SerialUSB.available() == 0);
                SerialUSB.read();

                float offset = readSensor(i);
                offsetVals[i] = offset;

                SerialUSB.print("Max ");
                SerialUSB.println(sensorOrder[i]);
                
                while(SerialUSB.available() == 0);
                SerialUSB.read();

                float high = readSensor(i);
                float delta = high - offset;
                //let's avoid db0
                if(delta == 0) {
                    scaleVals[i] = 1;
                }
                else {
                    scaleVals[i] = 1 / delta;
                }
            }
        }
    };
}