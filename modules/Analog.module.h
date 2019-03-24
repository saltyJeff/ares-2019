namespace Analog {
    extern uint16_t press0;
    extern uint16_t press1;
    extern uint16_t press2;
    extern uint16_t press3;
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