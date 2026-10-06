#pragma once

#include "Streaming/ICameraStreamFactory.h"

namespace AxiomVms
{
    class GStreamerCameraStreamFactory final : public ICameraStreamFactory
    {
    public:
        std::unique_ptr<ICameraStream> CreateCameraStream(const Camera& camera) override;
    };
}