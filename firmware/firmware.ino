#include "RocketModule.h"
#include "rocket.h"
bool isOn = true;

extern void preWarmup();
extern void warmup();
extern void refresh();

void setup() {
	SerialUSB.begin(115200);
	uint32_t serialAwaitStart = millis();
	while(millis() - serialAwaitStart < 3000 && !SerialUSB);
	SerialUSB.println("Congratulations on your code compiling");
	preWarmup();
	warmup();
	SerialUSB.println("Flushing remaining bytes");
	uint32_t flushStart = millis();
	while(millis() - flushStart < 2000 && SerialUSB.read() > 0);
	pinMode(LED_BUILTIN, OUTPUT);
	SerialUSB.println("Setup function complete, beginning telemetry");
}
uint32_t totalRefresh = 0;
bool highLoop = true;
void loop() {
	if(isOn) {
		if(highLoop) {
			digitalWrite(LED_BUILTIN, HIGH);
		}
		else {
			digitalWrite(LED_BUILTIN, LOW);
		}
		uint32_t startTime = millis();
		refresh();
		totalRefresh = millis() - startTime;
		highLoop = !highLoop;
	}
	runCli();
}