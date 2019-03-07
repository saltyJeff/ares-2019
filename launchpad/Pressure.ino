#include "rocket.h"
#include "RocketModule.h"
namespace Pressure {
    float &cc = Rocket::data.Pressure_cc;
    float &fuel = Rocket::data.Pressure_fuel;
    float &ox = Rocket::data.Pressure_ox;
    float &press = Rocket::data.Pressure_press;

    class PressureModule: public Rocket::RocketModule {
        virtual bool warmup() {
            pinMode(A2, INPUT_PULLUP);
            pinMode(A3, INPUT_PULLUP);
            pinMode(A4, INPUT_PULLUP);
            pinMode(A5, INPUT_PULLUP);
            delay(10);
            return true;
        }
        virtual void refresh() {
            cc = analogRead(A2) / 1024.0;
            fuel = analogRead(A3) / 1024.0;
            ox = analogRead(A4) / 1024.0;
            press = analogRead(A5) / 1024.0;
        }
    };
    PressureModule module;
    Rocket::RocketModule *handler = &module;
}