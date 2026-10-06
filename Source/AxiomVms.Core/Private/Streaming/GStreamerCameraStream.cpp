#include "Streaming/GStreamerCameraStream.h"

#include "Core/Logging/Log.h"

#include <gst/gst.h>

#include <chrono>
#include <mutex>
#include <thread>
#include <utility>

namespace AxiomVms
{
    GStreamerCameraStream::GStreamerCameraStream(std::string streamUrl)
        : StreamUrl_(std::move(streamUrl))
    {
        gst_init(nullptr, nullptr);
    }

    GStreamerCameraStream::~GStreamerCameraStream()
    {
        Stop();
        Cleanup();
    }

    bool GStreamerCameraStream::Start()
    {
        std::scoped_lock lock(Mutex_);

        if (Running_)
        {
            return false;
        }

        GError* error = nullptr;

        const std::string pipelineDescription =
            "rtspsrc location=\"" + StreamUrl_ +
            "\" latency=200 "
            "! rtph264depay "
            "! h264parse "
            "! fakesink";

        Pipeline_ = gst_parse_launch(pipelineDescription.c_str(), &error);

        if (error != nullptr)
        {
            AXIOM_LOG_ERROR("Failed to parse GStreamer pipeline for '{}': {}", StreamUrl_, error->message);

            g_clear_error(&error);
        }

        if (Pipeline_ == nullptr)
        {
            return false;
        }

        Bus_ = gst_element_get_bus(Pipeline_);

        if (Bus_ == nullptr)
        {
            AXIOM_LOG_ERROR("Failed to acquire GStreamer bus for '{}'.", StreamUrl_);

            Cleanup();
            return false;
        }

        const GstStateChangeReturn result = gst_element_set_state(Pipeline_, GST_STATE_PLAYING);

        if (result == GST_STATE_CHANGE_FAILURE)
        {
            AXIOM_LOG_ERROR("Failed to start GStreamer pipeline for '{}'.", StreamUrl_);

            Cleanup();
            return false;
        }

        Running_ = true;

        BusThread_ = std::jthread(
            [this](const std::stop_token &stopToken)
            {
                BusWorker(stopToken);
            });

        AXIOM_LOG_INFO("GStreamer pipeline starting for '{}'.", StreamUrl_);

        return true;
    }

    bool GStreamerCameraStream::Stop()
    {
        if (!Running_ && Pipeline_ == nullptr)
        {
            return false;
        }

        if (BusThread_.joinable())
        {
            BusThread_.request_stop();
            BusThread_.join();
        }

        std::scoped_lock lock(Mutex_);

        Running_ = false;

        Cleanup();

        AXIOM_LOG_INFO("GStreamer pipeline stopped for '{}'.", StreamUrl_);

        return true;
    }

    bool GStreamerCameraStream::IsRunning() const
    {
        return Running_.load();
    }

    void GStreamerCameraStream::BusWorker(const std::stop_token &stopToken)
    {
        GstBus* bus = nullptr;

        {
            std::scoped_lock lock(Mutex_);

            if (Bus_ == nullptr)
            {
                Running_ = false;
                return;
            }

            bus = GST_BUS(gst_object_ref(Bus_));
        }

        while (!stopToken.stop_requested())
        {
            GstMessage* message = gst_bus_timed_pop(bus, 100 * GST_MSECOND);

            if (message == nullptr)
            {
                continue;
            }

            const bool keepRunning = ProcessBusMessage(message);

            gst_message_unref(message);

            if (!keepRunning)
            {
                break;
            }
        }

        gst_object_unref(bus);
    }

    bool GStreamerCameraStream::ProcessBusMessage(GstMessage* message)
    {
        switch (GST_MESSAGE_TYPE(message))
        {
            case GST_MESSAGE_ERROR:
            {
                PrintBusError(message);

                Running_ = false;

                return false;
            }

            case GST_MESSAGE_EOS:
            {
                AXIOM_LOG_WARNING("GStreamer stream '{}' reached end-of-stream.", StreamUrl_);

                Running_ = false;

                return false;
            }

            case GST_MESSAGE_STATE_CHANGED:
            {
                GstElement* pipeline = nullptr;

                {
                    std::scoped_lock lock(Mutex_);
                    pipeline = Pipeline_;
                }

                if (pipeline != nullptr &&GST_MESSAGE_SRC(message) == GST_OBJECT(pipeline))
                {
                    GstState oldState;
                    GstState newState;
                    GstState pendingState;

                    gst_message_parse_state_changed(
                        message,
                        &oldState,
                        &newState,
                        &pendingState);

                    AXIOM_LOG_DEBUG(
                        "Pipeline '{}' changed state from {} to {}.",
                        StreamUrl_,
                        gst_element_state_get_name(oldState),
                        gst_element_state_get_name(newState));

                    if (newState == GST_STATE_PLAYING)
                    {
                        AXIOM_LOG_INFO("Camera stream '{}' reached PLAYING.", StreamUrl_);
                    }
                }

                break;
            }

            default:
                break;
        }

        return Running_;
    }

    void GStreamerCameraStream::PrintBusError(GstMessage* message)
    {
        GError* error = nullptr;
        gchar* debugInfo = nullptr;

        gst_message_parse_error( message,&error, &debugInfo);

        AXIOM_LOG_ERROR(
            "GStreamer error from '{}': {}",
            GST_OBJECT_NAME(message->src),
            error != nullptr ? error->message : "Unknown error");

        if (debugInfo != nullptr)
        {
            AXIOM_LOG_DEBUG("GStreamer debug information: {}", debugInfo);
        }

        g_clear_error(&error);
        g_free(debugInfo);
    }

    void GStreamerCameraStream::Cleanup()
    {
        if (Pipeline_ != nullptr)
        {
            gst_element_set_state(Pipeline_, GST_STATE_NULL);
        }

        if (Bus_ != nullptr)
        {
            gst_object_unref(Bus_);
            Bus_ = nullptr;
        }

        if (Pipeline_ != nullptr)
        {
            gst_object_unref(Pipeline_);
            Pipeline_ = nullptr;
        }

        Running_ = false;
    }

    CameraStreamStats GStreamerCameraStream::GetStats() const
    {
        return CameraStreamStats{};
    }
}
