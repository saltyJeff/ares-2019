#include "rocket.h"
const __FlashStringHelper *Rocket::NAME_CSV = F("BMP_altitude,MPU_accelX,MPU_accelY,MPU_accelZ,MPU_gyroX,MPU_gyroY,MPU_gyroZ,MPU_magX,MPU_magY,MPU_magZ,timestamp");
const __FlashStringHelper *Rocket::TYPE_CSV = F("float,float,float,float,float,float,float,float,float,float,uint32_t");
const __FlashStringHelper *Rocket::MODULE_CSV = F("BMP,MPU,Radio,SdCard");
const __FlashStringHelper *Rocket::MODULE_NAMES[4] = {F("BMP"), F("MPU"), F("Radio"), F("SdCard")};
