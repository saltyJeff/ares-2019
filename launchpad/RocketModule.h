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
};