#include "rocket.h"
#define FS(x) (const __FlashStringHelper*)(x)

const char NameText[] PROGMEM = {"BMP_altitude,BMP_temperature,BMP_pressure,BNO_w,BNO_x,BNO_y,BNO_z,MPU_accelX,MPU_accelY,MPU_accelZ,Pressure_cc,Pressure_fuel,Pressure_ox,Pressure_press,timestamp"};
const char TypeText[] PROGMEM = {"float,float,float,float,float,float,float,float,float,float,float,float,float,float,uint32_t"};
const char ModuleText[] PROGMEM = {"BMP,BNO,MPU,Pressure,Radio,SdCard"};
const __FlashStringHelper *Rocket::NAME_CSV = FS(NameText);
const __FlashStringHelper *Rocket::TYPE_CSV = FS(TypeText);
const __FlashStringHelper *Rocket::MODULE_CSV = FS(ModuleText);
const char ModuleBMPText[] PROGMEM = {"BMP"};
const char ModuleBNOText[] PROGMEM = {"BNO"};
const char ModuleMPUText[] PROGMEM = {"MPU"};
const char ModulePressureText[] PROGMEM = {"Pressure"};
const char ModuleRadioText[] PROGMEM = {"Radio"};
const char ModuleSdCardText[] PROGMEM = {"SdCard"};
const __FlashStringHelper *Rocket::MODULE_NAMES[6] = {
	FS(ModuleBMPText),
	FS(ModuleBNOText),
	FS(ModuleMPUText),
	FS(ModulePressureText),
	FS(ModuleRadioText),
	FS(ModuleSdCardText)
};
