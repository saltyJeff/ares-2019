#include "rocket.h"
#define FS(x) (const __FlashStringHelper*)(x)

const char NameText[] PROGMEM = {"BMP_altitude,BMP_temperature,BMP_pressure,MPU_accelX,MPU_accelY,MPU_accelZ,MPU_gyroX,MPU_gyroY,MPU_gyroZ,MPU_magX,MPU_magY,MPU_magZ,timestamp"};
const char TypeText[] PROGMEM = {"float,float,float,float,float,float,float,float,float,float,float,float,uint32_t"};
const char ModuleText[] PROGMEM = {"BMP,MPU,Radio,SdCard"};
const __FlashStringHelper *Rocket::NAME_CSV = FS(NameText);
const __FlashStringHelper *Rocket::TYPE_CSV = FS(TypeText);
const __FlashStringHelper *Rocket::MODULE_CSV = FS(ModuleText);
const char ModuleBMPText[] PROGMEM = {"BMP"};
const char ModuleMPUText[] PROGMEM = {"MPU"};
const char ModuleRadioText[] PROGMEM = {"Radio"};
const char ModuleSdCardText[] PROGMEM = {"SdCard"};
const __FlashStringHelper *Rocket::MODULE_NAMES[4] = {
	FS(ModuleBMPText),
	FS(ModuleMPUText),
	FS(ModuleRadioText),
	FS(ModuleSdCardText)
};
