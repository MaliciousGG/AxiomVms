#pragma once

#include <cstdint>
#include <string>

namespace AxiomVms
{
    enum class ECameraState : std::uint8_t
    {
        None,
        Booting,
        Connecting,
        Connected,
        Disconnected,
        Error
    };

    enum class ERecordingState : std::uint8_t
    {
        Stopped,
        Starting,
        Recording,
        Stopping,
        Error
    };

    struct Camera
    {
        std::uint64_t Id{};
        std::string Name;
        std::string StreamUrl;

        ECameraState State{ECameraState::None};
        ERecordingState RecordingState{ERecordingState::Stopped};
    };
}