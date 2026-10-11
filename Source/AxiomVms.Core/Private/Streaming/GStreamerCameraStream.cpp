#include "Streaming/GStreamerCameraStream.h"

#include "Core/Logging/Log.h"

#include <gst/gst.h>

#include <chrono>
#include <mutex>
#include <thread>
#include <utility>

namespace Axiom
{
    GStreamerCameraStream::GStreamerCameraStream(std::string InStreamUrl)
        : StreamUrl_(std::move(InStreamUrl))
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
        std::scoped_lock ScopeLock(Mutex_);

        if (Running_)
        {
            return false;
        }

        GError* Error = nullptr;

        const std::string PipelineDescription =
            "rtspsrc location=\"" + StreamUrl_ +
            "\" latency=200 "
            "! rtph264depay "
            "! h264parse "
            "! fakesink";

        Pipeline_ = gst_parse_launch(PipelineDescription.c_str(), &Error);

        if (Error != nullptr)
        {
            AXIOM_LOG_ERROR("Failed to parse GStreamer pipeline for '{}': {}", StreamUrl_, Error->message);

            g_clear_error(&Error);
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

        if (const GstStateChangeReturn Result = gst_element_set_state(Pipeline_, GST_STATE_PLAYING);
            Result == GST_STATE_CHANGE_FAILURE)
        {
            AXIOM_LOG_ERROR("Failed to start GStreamer pipeline for '{}'.", StreamUrl_);

            Cleanup();
            return false;
        }

        Running_ = true;

        BusThread_ = std::jthread([this](const std::stop_token &StopToken)
        {
                BusWorker(StopToken);
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

        std::scoped_lock ScopeLock(Mutex_);

        Running_ = false;

        Cleanup();

        AXIOM_LOG_INFO("GStreamer pipeline stopped for '{}'.", StreamUrl_);

        return true;
    }

    bool GStreamerCameraStream::IsRunning() const
    {
        return Running_.load();
    }

    void GStreamerCameraStream::BusWorker(const std::stop_token &StopToken)
    {
        GstBus* Bus = nullptr;

        {
            std::scoped_lock ScopeLock(Mutex_);

            if (Bus_ == nullptr)
            {
                Running_ = false;

                return;
            }

            Bus = GST_BUS(gst_object_ref(Bus_));
        }

        while (!StopToken.stop_requested())
        {
            GstMessage* Message = gst_bus_timed_pop(Bus, 100 * GST_MSECOND);

            if (Message == nullptr)
            {
                continue;
            }

            const bool KeepRunning = ProcessBusMessage(Message);

            gst_message_unref(Message);

            if (!KeepRunning)
            {
                break;
            }
        }

        gst_object_unref(Bus);
    }

    bool GStreamerCameraStream::ProcessBusMessage(GstMessage* InMessage)
    {
        switch (GST_MESSAGE_TYPE(InMessage))
        {
            case GST_MESSAGE_ERROR:
            {
                PrintBusError(InMessage);

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
                GstElement* Pipeline = nullptr;

                {
                    std::scoped_lock ScopeLock(Mutex_);
                    Pipeline = Pipeline_;
                }

                if (Pipeline != nullptr && GST_MESSAGE_SRC(InMessage) == GST_OBJECT(Pipeline))
                {
                    GstState OldState;
                    GstState NewState;
                    GstState PendingState;

                    gst_message_parse_state_changed(
                        InMessage,
                        &OldState,
                        &NewState,
                        &PendingState);

                    AXIOM_LOG_DEBUG(
                        "Pipeline '{}' changed state from {} to {}.",
                        StreamUrl_,
                        gst_element_state_get_name(OldState),
                        gst_element_state_get_name(NewState));

                    if (NewState == GST_STATE_PLAYING)
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

    void GStreamerCameraStream::PrintBusError(GstMessage* InMessage)
    {
        GError* Error = nullptr;
        gchar* DebugInfo = nullptr;

        gst_message_parse_error(InMessage, &Error, &DebugInfo);

        AXIOM_LOG_ERROR(
            "GStreamer error from '{}': {}",
            GST_OBJECT_NAME(InMessage->src),
            Error != nullptr ? Error->message : "Unknown error");

        if (DebugInfo != nullptr)
        {
            AXIOM_LOG_DEBUG("GStreamer debug information: {}", DebugInfo);
        }

        g_clear_error(&Error);
        g_free(DebugInfo);
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
