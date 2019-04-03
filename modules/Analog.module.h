namespace Analog {
    extern uint16_t CC;
    extern uint16_t fuel;
    extern uint16_t OX;
    extern uint16_t press;
    class Handler: public Rocket::RocketModule {
    public:
        virtual bool warmup() {
            return true;
        }
        virtual void refresh() {
            CC = analogRead(A0);
            fuel = analogRead(A1);
            OX = analogRead(A2);
            press = analogRead(A3);
        }
    };
}