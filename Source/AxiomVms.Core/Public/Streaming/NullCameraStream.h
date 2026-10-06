#pragma once

#include "Streaming/ICameraStream.h"

namespace AxiomVms
{
    class NullCameraStream final : public ICameraStream
    {
    public:
        bool Start() override
        {
            if (Running_)
            {
                return false;
            }

            Running_ = true;

            return true;
        }

        bool Stop() override
        {
            if (!Running_)
            {
                return false;
            }

            Running_ = false;
            
            return true;
        }

        [[nodiscard]]
        bool IsRunning() const override
        {
            return Running_;
        }

        [[nodiscard]]
        CameraStreamStats GetStats() const override
        {
            return CameraStreamStats{};
        }

    private:
        bool Running_{false};
    };
}