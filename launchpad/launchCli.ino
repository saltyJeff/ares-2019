#include <Persist.h>
#include "rocket.h"
#include "launchCli.h"
#include "RocketFuncs.h"

//copy pasta but apparently RST is located at position 0
void(* resetFunc) (void) = 0;

//persistent settings (all bytes)
FlashStorage(enabledModules, byte);
FlashStorage(sdEnabled, bool);
FlashStorage(radioEnabled, bool);

byte enableByte = enabledModules.read(); //cache value to save a roundtrip

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
            enabledModules.write(identifier);
            for(byte mask = 0x80; mask; mask >>= 1) {
                if(mask & identifier) {
                    SerialUSB.print('1');
                }
                else {
                    SerialUSB.print('0');
                }
            }
            enableByte = identifier;
            SerialUSB.println();
            Rocket::preWarmup();
            Rocket::warmup();           
            break;
        case Rocket::GET_MODULES_EN:
            identifier = enabledModules.read();
            for(byte mask = 0x80; mask; mask >>= 1) {
                if(mask & identifier) {
                    SerialUSB.print('1');
                }
                else {
                    SerialUSB.print('0');
                }
            }
            enableByte = identifier;
            SerialUSB.println();           
            break;
        case Rocket::CALIBRATE:
            Rocket::callibrate();
            break;
        case Rocket::RESET:
            Rocket::preWarmup();
            Rocket::warmup();
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
            SerialUSB.print(F("pong: "));
            SerialUSB.println(millis());
            break;
        case Rocket::SHUTDOWN:
            Rocket::shutdown();
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
namespace Rocket {
    void preWarmup() {
        for(int i = 0; i < MODULE_NUM; i++) {
            if(!byteFlag(enableByte, i)) {
                continue;
            }
            SerialUSB.print(F("Pre-warming: "));
            SerialUSB.println(MODULE_NAMES[i]);
            handlers[i]->preWarmup();
        }
        SerialUSB.print(F("Pre-warmup complete"));
    }
    void warmup() {
        for(int i = 0; i < MODULE_NUM; i++) {
            if(!byteFlag(enableByte, i)) {
                continue;
            }
            SerialUSB.print(F("Warming up: "));
            SerialUSB.print(MODULE_NAMES[i]);
            if(handlers[i]->warmup()) {
                SerialUSB.println(F(" ...successful"));
            }
            else {
                SerialUSB.println(F(" ...unsuccessful, disabling"));
                enableByte |= 1 << (7 - i);
            }
        }
        SerialUSB.print(F("Warmup complete"));
    }
    void refresh() {
        for(int i = 0; i < MODULE_NUM; i++) {
            if(!byteFlag(enableByte, i)) {
                continue;
            }
            uint32_t startTime = millis();
            handlers[i]->refresh();
            refreshTimes[i] = millis() - startTime;
        }
        data.timestamp = millis();
    }
    void callibrate() {
        for(int i = 0; i < MODULE_NUM; i++) {
            if(!byteFlag(enableByte, i)) {
                continue;
            }
            SerialUSB.print(F("Callibrating: "));
            SerialUSB.println(MODULE_NAMES[i]);
            handlers[i]->callibrate();
        }
    }
    void shutdown() {
        for(int i = 0; i < MODULE_NUM; i++) {
            if(!byteFlag(enableByte, i)) {
                continue;
            }
            SerialUSB.print(F("Shutting down: "));
            SerialUSB.println(MODULE_NAMES[i]);
            handlers[i]->shutdown();
        }
    }
}
