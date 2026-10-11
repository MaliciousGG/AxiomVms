#pragma once

#include "Streaming/ICameraStream.h"

#include <atomic>
#include <mutex>
#include <gst/gst.h>
#include <stop_token>
#include <string>
#include <thread>

namespace Axiom
{
    class GStreamerCameraStream final : public ICameraStream
    {
    public:
        explicit GStreamerCameraStream(std::string InStreamUrl);
        ~GStreamerCameraStream() override;

        bool Start() override;
        bool Stop() override;

        [[nodiscard]]
        bool IsRunning() const override;

    private:
        void BusWorker(const std::stop_token &StopToken);

        bool ProcessBusMessage(GstMessage* InMessage);

        static void PrintBusError(GstMessage* InMessage);

        void Cleanup();

    public:
        [[nodiscard]] CameraStreamStats GetStats() const override;

    private:
        GstElement* Pipeline_{nullptr};
        GstBus* Bus_{nullptr};

        std::string StreamUrl_;

        std::atomic_bool Running_{false};

        std::jthread BusThread_;

        mutable std::mutex Mutex_;
    };
}