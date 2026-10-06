target_sources(
    AxiomVms.Core
    PRIVATE
        Private/Streaming/NullCameraStreamFactory.cpp
        Private/Streaming/GStreamerCameraStream.cpp
        Private/Streaming/GStreamerCameraStreamFactory.cpp

    PUBLIC
        Public/Streaming/CameraStreamStats.h
        Public/Streaming/ICameraStream.h
        Public/Streaming/ICameraStreamFactory.h
        Public/Streaming/NullCameraStream.h
        Public/Streaming/NullCameraStreamFactory.h
        Public/Streaming/GStreamerCameraStream.h
        Public/Streaming/GStreamerCameraStreamFactory.h
)