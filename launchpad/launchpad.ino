#include "rocket.h"
#include "RocketFuncs.h"

#ifdef __AVR__
#define SerialUSB Serial
#include <SoftwareSerial.h>
SoftwareSerial XBee(2,3);
#define Serial1 XBee
#endif
uint32_t totalRefresh = 0;
void setup() {
    SerialUSB.begin(115200); //default speed
    SerialUSB.flush();
    while(!SerialUSB) {}
    
    Rocket::preWarmup();
    Rocket::warmup();
    SerialUSB.println(F("Initial warmup complete"));
}
void loop() {
    uint32_t startTime = millis();
    Rocket::refresh();
    runCli();
    totalRefresh = millis() - startTime;
}