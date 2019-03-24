#pragma once

namespace Rocket {
    class RocketModule {
    public:
        virtual void preWarmup() {};
        virtual bool warmup() { return true; };
        virtual void refresh() = 0;
        virtual void callibrate() {};
        virtual void shutdown() {};
    };
};