#include <FlashStorage.h>
#include "launchCli.h"

void preWarmup();
void warmup();
void refresh();
void shutdown();

//persistent settings (all bytes)
FlashStorage(enabledModules, byte);

byte enabledByte = enabledModules.read(); //cache value to save a roundtrip

extern bool isOn;
uint32_t refreshTimes[Rocket::MODULE_NUM];
extern uint32_t totalRefresh;

void runCli() {
    int readCommand = SerialUSB.read();
    if(readCommand < 0) {
        return;
    }
    //SerialUSB.println(readCommand);
    byte identifier = 0;
    switch(readCommand) {
        case Rocket::META_FIELDS:
            SerialUSB.println(Rocket::NAME_CSV);
            break;
        case Rocket::META_TYPES:
            SerialUSB.println(Rocket::TYPE_CSV);
            break;
        case Rocket::META_MODULES:
            SerialUSB.println(Rocket::MODULE_CSV);
            break;
        case Rocket::SET_MODULES_EN:
            identifier = awaitArg();
            if(identifier != enabledByte) {
                enabledModules.write(identifier);
            }
            SerialUSB.println(identifier, BIN);
            enabledByte = identifier;
            SerialUSB.println();
            preWarmup();
            warmup();           
            break;
        case Rocket::GET_MODULES_EN:
            SerialUSB.println(enabledByte, BIN);
            SerialUSB.println();           
            break;
        case Rocket::CALIBRATE:
            identifier = awaitArg();
            calibrate(identifier);
            break;
        case Rocket::RESET:
            shutdown();
            preWarmup();
            warmup();
            isOn = true;
            break;
        case Rocket::BENCH:
            for(int i = 0; i < Rocket::MODULE_NUM - 1; i++) {
                SerialUSB.print(refreshTimes[i]);
                SerialUSB.print(',');
            }
            if(Rocket::MODULE_NUM > 0) {
                SerialUSB.println(refreshTimes[Rocket::MODULE_NUM - 1]);
            }
            SerialUSB.println(totalRefresh);
            break;
        case Rocket::PING:
            SerialUSB.print("pong: ");
            SerialUSB.println(millis());
            break;
        case Rocket::SHUTDOWN:
            shutdown();
            isOn = false;
            break;
    }
}
//holds until a byte comes through
byte awaitArg() {
    while(SerialUSB.available() == 0);
    return (byte) SerialUSB.read();
}
inline bool byteFlag(byte b, int idx) {
    return b & (1 << (7 - idx));
}

// This should be in a differnt file, but it requires the flashstorage settings and I don't want
// to dig thru docs to figure out what type of PTR i need
void preWarmup() {
    for(int i = 0; i < Rocket::MODULE_NUM; i++) {
        if(!byteFlag(enabledByte, i)) {
            continue;
        }
        SerialUSB.print("Pre-warming module: ");
        SerialUSB.println(Rocket::MODULE_NAMES[i]);
        Rocket::handlers[i]->preWarmup();
    }
    SerialUSB.println("Pre-warmup complete");
}
void warmup() {
    for(int i = 0; i < Rocket::MODULE_NUM ; i++) {
        if(!byteFlag(enabledByte, i)) {
            continue;
        }
        SerialUSB.print("Warming up: ");
        SerialUSB.println(Rocket::MODULE_NAMES[i]);
        if(Rocket::handlers[i]->warmup()) {
            SerialUSB.println(" ...successful");
        }
        else {
            SerialUSB.println(" ...unsuccessful, disabling");
            enabledByte &= ~(1 << (7 - i));
        }
    }
    SerialUSB.println("Warmup complete");
}
void refresh() {
    for(int i = 0; i < Rocket::MODULE_NUM; i++) {
        if(!byteFlag(enabledByte, i)) {
            continue;
        }
        //SerialUSB.print("Refreshing: ");
        //SerialUSB.println(Rocket::MODULE_NAMES[i]);
        uint32_t startTime = millis();
        Rocket::handlers[i]->refresh();
        refreshTimes[i] = millis() - startTime;
    }
    Rocket::data.timestamp = millis();
}
//an argument of 8 will calibrate everything
void calibrate(byte arg) {
    for(int i = 0; i < Rocket::MODULE_NUM; i++) {
        if(!byteFlag(enabledByte, i)) {
            continue;
        }
        if(arg == 8 || i == arg) {
            SerialUSB.print(F("Calibrating: "));
            SerialUSB.println(Rocket::MODULE_NAMES[i]);
            Rocket::handlers[i]->calibrate();
        }
    }
}
void shutdown() {
    for(int i = 0; i < Rocket::MODULE_NUM; i++) {
        if(!byteFlag(enabledByte, i)) {
            continue;
        }
        SerialUSB.print(F("Shutting down: "));
        SerialUSB.println(Rocket::MODULE_NAMES[i]);
        Rocket::handlers[i]->shutdown();
    }
    SerialUSB.println(F("Shut down. Goodbye!"));
}