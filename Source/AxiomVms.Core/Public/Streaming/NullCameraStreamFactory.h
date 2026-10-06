#pragma once

#include "Streaming/ICameraStreamFactory.h"
#include "Cameras/Camera.h"

namespace AxiomVms
{
    class NullCameraStreamFactory final : public ICameraStreamFactory
    {
    public:
        std::unique_ptr<ICameraStream> CreateCameraStream(const Camera& camera) override;
    };
}

