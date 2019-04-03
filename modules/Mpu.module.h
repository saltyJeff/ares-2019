///https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library/archive/master.zip
#include <SparkFunMPU9250-DMP.h>
#include <Wire.h>
#include <FlashStorage.h>

/*
 * an IIFE that gives the average of a 100 readings
 * M is the name of the method (calcAccel/calcGyro)
 * V is the name of the variable (ax/gx)
*/
#define AVG_100(M, V) \
    [&] { \
    float total = 0; \
    for(int i = 0; i < 100; i++) { \
        imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS); \
        total+= imu.M(imu.V); \
    } \
    return total / 100; \
}();

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

    struct CalibData {
        float offsets[6];
        float scales[3];
    };

    FlashStorage(calibData, CalibData);

    class Handler: public Rocket::RocketModule {
    public:
        float *offsetVals;
        float *scaleVals;
        CalibData calib;
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

            calib = calibData.read();

            offsetVals = calib.offsets;
            scaleVals = calib.scales;
            //make sure none of the scales are 0
            for(int i = 0; i < 3; i++) {
                if(scaleVals[i] == 0) {
                    scaleVals[i] = 1;
                }
            }

            return setupRight;
        }
        virtual void refresh() {
            //SerialUSB.println(Rocket::data.Mpu_accelX);
            if(!imu.dataReady()) {
                SerialUSB.println("no data from MPU");
                return;
            }
            imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
            accelX = (imu.calcAccel(imu.ax) - offsetVals[0]) * scaleVals[0]; // accelX is x-axis acceleration in g's
            accelY = (imu.calcAccel(imu.ay) - offsetVals[1]) * scaleVals[1]; // accelY is y-axis acceleration in g's
            accelZ = (imu.calcAccel(imu.ay) - offsetVals[2]) * scaleVals[2]; // accelZ is z-axis acceleration in g's

            gyroX = (imu.calcGyro(imu.gx) - offsetVals[3]); // gyroX is x-axis rotation in dps
            gyroY = (imu.calcGyro(imu.gy) - offsetVals[4]); // gyroY is y-axis rotation in dps
            gyroZ = (imu.calcGyro(imu.gz) - offsetVals[5]); // gyroZ is z-axis rotation in dps
            
            magX = imu.calcMag(imu.mx); // magX is x-axis magnetic field in uT
            magY = imu.calcMag(imu.my); // magY is y-axis magnetic field in uT
            magZ = imu.calcMag(imu.mz); // magZ is z-axis magnetic field in uTcc
        }
        float readSensor(int id) {
            SerialUSB.println("Reading sensor ");
            float f = 0;
            if(id == 0) {
                f = AVG_100(calcAccel, ax)
            }
            else if(id == 1) {
                f = AVG_100(calcAccel, ay)
            }
            else if(id == 2) {
                f = AVG_100(calcAccel, az)
            }
            else if(id == 3) {
                f = AVG_100(calcGyro, gx)
            }
            else if(id == 4) {
                f = AVG_100(calcGyro, gy)
            }
            else if(id == 5) {
                f = AVG_100(calcGyro, gz)
            }
            SerialUSB.println(f);
            return f;
        }
        virtual void calibrate() {
            SerialUSB.println("Calibrating the MPU. Press enter when you're done with the instruction");            
            const char *sensorOrder[] = {
                "accelX",
                "accelY",
                "accelZ",
                "gyroX",
                "gyroY",
                "gyroZ"
            };

            for(int i = 0; i < 6; i++) {
                SerialUSB.print("Null ");
                SerialUSB.println(sensorOrder[i]);
                
                while(SerialUSB.available() == 0);
                SerialUSB.read();

                float offset = readSensor(i);
                offsetVals[i] = offset;

                if(i >= 3) {
                    continue; //no maximums for the gyroscopes
                }

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
            calibData.write(calib);
            SerialUSB.println("Calibration complete");
        }
    };
}