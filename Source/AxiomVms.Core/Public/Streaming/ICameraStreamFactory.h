#pragma once

#include "Cameras/Camera.h"
#include "Streaming/ICameraStream.h"

#include <memory>

namespace AxiomVms
{
    class ICameraStreamFactory
    {
    public:
        virtual ~ICameraStreamFactory() = default;

        virtual std::unique_ptr<ICameraStream> CreateCameraStream(const Camera& camera) = 0;
    };
}