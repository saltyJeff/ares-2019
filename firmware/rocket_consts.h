namespace Rocket {
	constexpr char* NAME_CSV = "Mpu_accelX,Mpu_accelY,Mpu_accelZ,Mpu_gyroX,Mpu_gyroY,Mpu_gyroZ,Mpu_magX,Mpu_magY,Mpu_magZ,timestamp";
	constexpr char* TYPE_CSV = "float,float,float,float,float,float,float,float,float,uint32_t";
	constexpr char* MODULE_CSV = "Mpu,Radio,SdCard";
	constexpr char* Mpu_TEXT = "Mpu";
	constexpr char* Radio_TEXT = "Radio";
	constexpr char* SdCard_TEXT = "SdCard";
	constexpr char* MODULE_NAMES[3] = {
		Mpu_TEXT,
		Radio_TEXT,
		SdCard_TEXT
	};
}
