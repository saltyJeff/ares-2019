#pragma once
#include "RocketModule.h"
namespace Analog {
    uint16_t &press0 = Rocket::data.Analog_press0;
    uint16_t &press1 = Rocket::data.Analog_press1;
    uint16_t &press2 = Rocket::data.Analog_press2;
    uint16_t &press3 = Rocket::data.Analog_press3;
    class Handler: public Rocket::RocketModule {
    public:
        virtual bool warmup() {
            return true;
        }
        virtual void refresh() {
            press0 = analogRead(A0);
            press1 = analogRead(A1);
            press2 = analogRead(A2);
            press3 = analogRead(A3);
        }
    };
}