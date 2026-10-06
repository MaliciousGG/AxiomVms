#pragma once

#include <chrono>
#include <cstdint>

namespace AxiomVms
{
    struct CameraStreamStats
    {
        std::chrono::milliseconds Uptime{0};
        std::uint64_t FrameCount{0};
    };
}