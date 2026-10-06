#pragma once

#include "Camera.h"
#include "Streaming/ICameraStream.h"

#include <memory>

namespace AxiomVms
{
    class CameraSession
    {
    public:
        explicit CameraSession(Camera& camera,  std::unique_ptr<ICameraStream> stream);

        [[nodiscard]]
        bool Start() const;

        [[nodiscard]]
        bool Stop() const;

        [[nodiscard]]
        bool IsRunning() const;

        [[nodiscard]]
        CameraStreamStats GetStats() const;

    private:
        Camera& Camera_;
        std::unique_ptr<ICameraStream> Stream_;
    };
}