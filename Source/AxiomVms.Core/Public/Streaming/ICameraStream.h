#pragma once
#include "CameraStreamStats.h"

namespace AxiomVms
{
    class ICameraStream
    {
    public:
        virtual ~ICameraStream() = default;

        virtual bool Start() = 0;
        virtual bool Stop() = 0;

        [[nodiscard]]
        virtual bool IsRunning() const = 0;

        [[nodiscard]]
        virtual CameraStreamStats GetStats() const = 0;
    };
}
