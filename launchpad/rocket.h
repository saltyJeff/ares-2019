#pragma once
#include <WString.h>
#include "RocketModule.h"

namespace BMP {
	extern Rocket::RocketModule *handler;
}
namespace MPU {
	extern Rocket::RocketModule *handler;
}
namespace Radio {
	extern Rocket::RocketModule *handler;
}
namespace SdCard {
	extern Rocket::RocketModule *handler;
}

namespace Rocket {
	#pragma pack(1)
	struct ROCKET_DATA {
		float BMP_altitude;
		float BMP_temperature;
		float BMP_pressure;
		float MPU_accelX;
		float MPU_accelY;
		float MPU_accelZ;
		float MPU_gyroX;
		float MPU_gyroY;
		float MPU_gyroZ;
		float MPU_magX;
		float MPU_magY;
		float MPU_magZ;
		uint32_t timestamp;
	};
	ROCKET_DATA data;

	const char *DATA_START = (char *)&data;
	const int DATA_LEN = sizeof(data);
	extern const __FlashStringHelper *NAME_CSV;
	extern const __FlashStringHelper *TYPE_CSV;
	extern const __FlashStringHelper *MODULE_CSV;

	const int MODULE_NUM = 4;
	const int BMP_ID = 0;
	const int MPU_ID = 1;
	const int Radio_ID = 2;
	const int SdCard_ID = 3;
	Rocket::RocketModule *handlers[] = {BMP::handler, MPU::handler, Radio::handler, SdCard::handler};
	extern const __FlashStringHelper *MODULE_NAMES[4];
}

