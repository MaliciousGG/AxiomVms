#include "Cameras/CameraSession.h"

namespace AxiomVms
{
    
    CameraSession::CameraSession(Camera& camera, std::unique_ptr<ICameraStream> stream)
        : Camera_(camera)
        , Stream_(std::move(stream))
    {}

    bool CameraSession::Start() const
    {
        if (Camera_.State == ECameraState::Connecting || Camera_.State == ECameraState::Connected)
        {
            return false;
        }

        Camera_.State = ECameraState::Connecting;

        if (!Stream_->Start())
        {
            Camera_.State = ECameraState::Error;

            return false;
        }

        Camera_.State = ECameraState::Connected;

        return true;
    }

    bool CameraSession::Stop() const
    {
        if (Camera_.State == ECameraState::Disconnected || Camera_.State == ECameraState::None)
        {
            return false;
        }

        if (!Stream_->Stop())
        {
            Camera_.State = ECameraState::Error;

            return false;
        }

        Camera_.State = ECameraState::Disconnected;

        return true;
    }

    bool CameraSession::IsRunning() const
    {
        return Stream_->IsRunning();
    }

    CameraStreamStats CameraSession::GetStats() const
    {
        return Stream_->GetStats();
    }
}
