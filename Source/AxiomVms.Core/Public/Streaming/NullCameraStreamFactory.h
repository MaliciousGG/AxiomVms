#pragma once

#include "Streaming/ICameraStreamFactory.h"
#include "Cameras/Camera.h"

namespace Axiom
{
    class NullCameraStreamFactory final : public ICameraStreamFactory
    {
    public:
        std::unique_ptr<ICameraStream> CreateCameraStream(const Camera& camera) override;
    };
}

