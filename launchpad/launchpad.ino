#include "rocket.h"
#include "RocketFuncs.h"

uint32_t totalRefresh = 0;
void setup() {
    SerialUSB.begin(115200); //default speed
    SerialUSB.flush();
    while(!SerialUSB) {}
    
    Rocket::preWarmup();
    Rocket::warmup();
    SerialUSB.println(F("Warmed up"));
}
void loop() {
    uint32_t startTime = millis();
    Rocket::refresh();
    runCli();
    totalRefresh = millis() - startTime;
}