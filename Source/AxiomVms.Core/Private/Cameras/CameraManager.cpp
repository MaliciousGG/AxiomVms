#include "Cameras/CameraManager.h"
#include "Cameras/CameraSession.h"
#include "Streaming/ICameraStreamFactory.h"

#include <memory>
#include <ranges>
#include <utility>

namespace Axiom
{
    CameraManager::CameraManager(std::unique_ptr<ICameraStreamFactory> InStreamFactory)
        : StreamFactory_(std::move(InStreamFactory))
    {
    }

    CameraManager::~CameraManager() = default;

    std::uint64_t CameraManager::AddCamera(std::string InCameraName, std::string InStreamUrl)
    {
        for (const auto &camera: Cameras_ | std::views::values)
        {
            if (camera.Name == InCameraName && camera.StreamUrl == InStreamUrl)
            {
                return 0;
            }
        }

        const std::uint64_t Id = NextCameraId();

        Camera NewCamera
        {
            .Id = Id,
            .Name = std::move(InCameraName),
            .StreamUrl = std::move(InStreamUrl)
        };

        auto [Iterator, Inserted] = Cameras_.emplace(Id, std::move(NewCamera));

        if (!Inserted)
        {
            return 0;
        }

        auto Stream = StreamFactory_->CreateCameraStream(Iterator->second);

        if (!Stream)
        {
            Cameras_.erase(Iterator);

            return 0;
        }

        Sessions_.emplace(Id, std::make_unique<CameraSession>(Iterator->second, std::move(Stream)));

        return Id;
    }

    bool CameraManager::RemoveCamera(const std::uint64_t OutCameraName)
    {
        Sessions_.erase(OutCameraName);

        return Cameras_.erase(OutCameraName) > 0;
    }

    Camera* CameraManager::FindCamera(const std::uint64_t CameraId)
    {
        const auto Iterator = Cameras_.find(CameraId);

        if (Iterator == Cameras_.end())
        {
            return nullptr;
        }

        return &Iterator->second;
    }


    const Camera* CameraManager::FindCamera(const std::uint64_t CameraId) const
    {
        const auto Iterator = Cameras_.find(CameraId);

        if (Iterator == Cameras_.end())
        {
            return nullptr;
        }

        return &Iterator->second;
    }

    Camera* CameraManager::FindCamera(const std::string& CameraName)
    {
        for (auto &Camera: Cameras_ | std::views::values)
        {
            if (Camera.Name == CameraName)
            {
                return &Camera;
            }
        }

        return nullptr;
    }

    const Camera* CameraManager::FindCamera(const std::string& CameraName) const
    {        
        for (const auto &Camera: Cameras_ | std::views::values)
        {
            if (Camera.Name == CameraName)
            {
                return &Camera;
            }
        }

        return nullptr;
    }

    std::size_t CameraManager::GetCameraCount() const
    {
        return Cameras_.size();
    }

    bool CameraManager::StartCamera(const std::uint64_t CameraId)
    {
        const auto Iterator = Sessions_.find(CameraId);

        if (Iterator == Sessions_.end())
        {
            return false;
        }

        return Iterator->second->Start();
    }

    bool CameraManager::StopCamera(const std::uint64_t CameraId)
    {
        const auto Iterator = Sessions_.find(CameraId);

        if (Iterator == Sessions_.end())
        {
            return false;
        }

        return Iterator->second->Stop();
    }

    std::uint64_t CameraManager::NextCameraId()
    {
        return ++NextId_;
    }
}