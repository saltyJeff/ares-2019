#pragma once

#include "RocketModule.h"

namespace Rocket {
	#pragma pack(push, 1)
	struct ROCKET_DATA {
		byte start1 = 0xff;
		byte start2 = 0xff;
		float Mpu_accelX;
		float Mpu_accelY;
		float Mpu_accelZ;
		float Mpu_gyroX;
		float Mpu_gyroY;
		float Mpu_gyroZ;
		float Mpu_magX;
		float Mpu_magY;
		float Mpu_magZ;
		uint32_t timestamp;
		byte end1 = 0xa4;
		byte end2 = 0x55;
	};
	#pragma pack(pop)
	static ROCKET_DATA data;

	const char *DATA_START = (char *)&data;
	const int DATA_LEN = sizeof(ROCKET_DATA);

	const int MODULE_NUM = 3;
	const int Mpu_ID = 0;
	const int Radio_ID = 1;
	const int SdCard_ID = 2;
}

#include "rocket_consts.h"

#include "Mpu.h"
#include "Radio.h"
#include "SdCard.h"

namespace Rocket {
	static Mpu::MpuClass mpuInstance;
	static Radio::RadioClass radioInstance;
	static SdCard::SdCardClass sdInstance;
	static RocketModule *handlers[] = {&mpuInstance, &radioInstance, &sdInstance};
}


