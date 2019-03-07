#pragma once
#include <WString.h>

namespace Rocket {
    class RocketModule {
    public:
        virtual void preWarmup() {};
        virtual bool warmup() { return true; };
        virtual void refresh() {};
        virtual void callibrate() {};
        virtual void shutdown() {};
    };
    const byte startDelim[2] = {0xFF, 0xFF};
    const byte endDelim[2] = {0xA4, 0x55};
};