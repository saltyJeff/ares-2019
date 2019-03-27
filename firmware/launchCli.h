#pragma once
namespace Rocket {
    //everything has 1 offset so I don't need to deal with nullbytes
    const byte META_FIELDS = 0x1;
    const byte META_TYPES = 0x2;
    const byte META_MODULES = 0x3;
    const byte SET_MODULES_EN = 0x4;
    const byte GET_MODULES_EN = 0x5;
    const byte CALIBRATE = 0x6;
    const byte RESET = 0x7;
    const byte BENCH = 0x8;
    const byte SHUTDOWN = 0x9;
    const byte PING = 0xA; //will be newline
}