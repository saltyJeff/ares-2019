#pragma once
#include <WString.h>
#include "RocketModule.h"

namespace BMP {
	extern Rocket::RocketModule *handler;
}
namespace BNO {
	extern Rocket::RocketModule *handler;
}
namespace MPU {
	extern Rocket::RocketModule *handler;
}
namespace Pressure {
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
		byte start1 = 0xff;
		byte start2 = 0xff;
		float BMP_altitude;
		float BMP_temperature;
		float BMP_pressure;
		float BNO_w;
		float BNO_x;
		float BNO_y;
		float BNO_z;
		float MPU_accelX;
		float MPU_accelY;
		float MPU_accelZ;
		float Pressure_cc;
		float Pressure_fuel;
		float Pressure_ox;
		float Pressure_press;
		uint32_t timestamp;
		byte end1 = 0xa4;
		byte end2 = 0x55;
	};
	ROCKET_DATA data;

	const char *DATA_START = (char *)&data;
	const int DATA_LEN = sizeof(data);
	extern const __FlashStringHelper *NAME_CSV;
	extern const __FlashStringHelper *TYPE_CSV;
	extern const __FlashStringHelper *MODULE_CSV;

	const int MODULE_NUM = 6;
	const int BMP_ID = 0;
	const int BNO_ID = 1;
	const int MPU_ID = 2;
	const int Pressure_ID = 3;
	const int Radio_ID = 4;
	const int SdCard_ID = 5;
	Rocket::RocketModule *handlers[] = {BMP::handler, BNO::handler, MPU::handler, Pressure::handler, Radio::handler, SdCard::handler};
	extern const __FlashStringHelper *MODULE_NAMES[6];
}

