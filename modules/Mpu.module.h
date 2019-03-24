///https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library/archive/master.zip
#include <SparkFunMPU9250-DMP.h>
#include <Wire.h>
namespace Mpu {
    extern float accelX;
    extern float accelY;
    extern float accelZ;
    extern float gyroX;
    extern float gyroY;
    extern float gyroZ;
    extern float magX;
    extern float magY;
    extern float magZ;

    class Handler: public Rocket::RocketModule {
    public:
        MPU9250_DMP imu;
        virtual bool warmup() {
            bool setupRight = imu.begin() == INV_SUCCESS;
            if(!setupRight) {
                return false;
            }
            imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
            imu.setGyroFSR(1000);
            imu.setAccelFSR(16);
            imu.setLPF(5);
            imu.setSampleRate(500);
            imu.setCompassSampleRate(10);
            return setupRight;
        }
        virtual void refresh() {
            //SerialUSB.println(Rocket::data.Mpu_accelX);
            if(!imu.dataReady()) {
                SerialUSB.println("no data from MPU");
                return;
            }
            imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
            accelX = imu.calcAccel(imu.ax); // accelX is x-axis acceleration in g's
            accelY = imu.calcAccel(imu.ay); // accelY is y-axis acceleration in g's
            accelZ = imu.calcAccel(imu.az); // accelZ is z-axis acceleration in g's

            gyroX = imu.calcGyro(imu.gx); // gyroX is x-axis rotation in dps
            gyroY = imu.calcGyro(imu.gy); // gyroY is y-axis rotation in dps
            gyroZ = imu.calcGyro(imu.gz); // gyroZ is z-axis rotation in dps
            
            magX = imu.calcMag(imu.mx); // magX is x-axis magnetic field in uT
            magY = imu.calcMag(imu.my); // magY is y-axis magnetic field in uT
            magZ = imu.calcMag(imu.mz); // magZ is z-axis magnetic field in uTcc
        }
    };
}