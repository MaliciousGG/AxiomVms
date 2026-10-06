#pragma once

#include "Streaming/ICameraStream.h"

#include <atomic>
#include <mutex>
#include <gst/gst.h>
#include <stop_token>
#include <string>
#include <thread>

namespace AxiomVms
{
    class GStreamerCameraStream final : public ICameraStream
    {
    public:
        explicit GStreamerCameraStream(std::string streamUrl);
        ~GStreamerCameraStream() override;

        bool Start() override;
        bool Stop() override;

        [[nodiscard]]
        bool IsRunning() const override;

    private:
        void BusWorker(const std::stop_token &stopToken);

        bool ProcessBusMessage(GstMessage* message);

        static void PrintBusError(GstMessage* message);

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