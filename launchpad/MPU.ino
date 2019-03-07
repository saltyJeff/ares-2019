#include "rocket.h"
#include "RocketModule.h"
///https://github.com/saltyJeff/I2C_MPU6050/archive/master.zip
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
namespace MPU {
    float &accelX = Rocket::data.MPU_accelX;
    float &accelY = Rocket::data.MPU_accelY;
    float &accelZ = Rocket::data.MPU_accelZ;

    MPU6050 mpu;
    int16_t intXAccel, intYAccel, intZAccel;
    class MpuModule: public Rocket::RocketModule {
    public:
        virtual bool warmup() {
            mpu.initialize();
            delay(100);
            bool setupRight = mpu.testConnection();
            if(!setupRight) {
                return false;
            }
            mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
            return true;
        }
        virtual void refresh() {
            mpu.getAcceleration(&intXAccel, &intYAccel, &intZAccel);
            accelX = intXAccel / 2048.0;
            accelY = intYAccel / 2048.0;
            accelZ = intZAccel / 2048.0;
        }
    };
    MpuModule module;
    Rocket::RocketModule *handler = &module;
}