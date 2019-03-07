namespace Pressure {
    extern float cc;
    extern float fuel;
    extern float ox;
    extern float press;

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