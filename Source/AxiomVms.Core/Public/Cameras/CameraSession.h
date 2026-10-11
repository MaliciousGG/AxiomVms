#pragma once

#include "Camera.h"
#include "Streaming/ICameraStream.h"

#include <memory>

namespace Axiom
{
    class CameraSession
    {
    public:
        explicit CameraSession(Camera& InCamera,  std::unique_ptr<ICameraStream> InStream);

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