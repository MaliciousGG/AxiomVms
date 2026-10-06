target_sources(
    AxiomVms.Core
    PRIVATE
        Private/Cameras/CameraManager.cpp
        Private/Cameras/CameraSession.cpp

    PUBLIC
        Public/Cameras/Camera.h
        Public/Cameras/CameraManager.h
        Public/Cameras/CameraSession.h
)