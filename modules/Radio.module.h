namespace Radio {
    //configured for the razor imu
    class Handler: public Rocket::RocketModule {
    public:
        virtual bool warmup() {
            //it's all RX TX, we can only hope
            Serial1.begin(9600);
            while(!Serial1);
            return true;
        }
        virtual void refresh() {
            Serial1.write(Rocket::DATA_START, Rocket::DATA_LEN);
        }
    };
}