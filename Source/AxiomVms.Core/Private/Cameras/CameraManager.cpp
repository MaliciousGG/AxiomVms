#include "Cameras/CameraManager.h"
#include "Cameras/CameraSession.h"
#include "Streaming/ICameraStreamFactory.h"

#include <memory>
#include <ranges>
#include <utility>

namespace AxiomVms
{
    CameraManager::CameraManager(std::unique_ptr<ICameraStreamFactory> streamFactory)
        : StreamFactory_(std::move(streamFactory))
    {
    }

    CameraManager::~CameraManager() = default;

    std::uint64_t CameraManager::AddCamera(std::string name, std::string streamUrl)
    {
        for (const auto &camera: Cameras_ | std::views::values)
        {
            if (camera.Name == name && camera.StreamUrl == streamUrl)
            {
                return 0;
            }
        }

        const std::uint64_t id = NextCameraId();

        Camera camera
        {
            .Id = id,
            .Name = std::move(name),
            .StreamUrl = std::move(streamUrl)
        };

        auto [it, inserted] = Cameras_.emplace(id, std::move(camera));

        if (!inserted)
        {
            return 0;
        }

        auto stream = StreamFactory_->CreateCameraStream(it->second);

        if (!stream)
        {
            Cameras_.erase(it);

            return 0;
        }

        Sessions_.emplace(id, std::make_unique<CameraSession>(it->second, std::move(stream)));

        return id;
    }

    bool CameraManager::RemoveCamera(const std::uint64_t id)
    {
        Sessions_.erase(id);

        return Cameras_.erase(id) > 0;
    }

    Camera* CameraManager::FindCamera(const std::uint64_t id)
    {
        const auto it = Cameras_.find(id);

        if (it == Cameras_.end())
        {
            return nullptr;
        }

        return &it->second;
    }


    const Camera* CameraManager::FindCamera(const std::uint64_t id) const
    {
        const auto it = Cameras_.find(id);

        if (it == Cameras_.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    Camera* CameraManager::FindCamera(const std::string& name)
    {
        for (auto &camera: Cameras_ | std::views::values)
        {
            if (camera.Name == name)
            {
                return &camera;
            }
        }

        return nullptr;
    }

    const Camera* CameraManager::FindCamera(const std::string& name) const
    {        
        for (const auto &camera: Cameras_ | std::views::values)
        {
            if (camera.Name == name)
            {
                return &camera;
            }
        }

        return nullptr;
    }

    std::size_t CameraManager::GetCameraCount() const
    {
        return Cameras_.size();
    }

    bool CameraManager::StartCamera(const std::uint64_t id)
    {
        const auto it = Sessions_.find(id);

        if (it == Sessions_.end())
        {
            return false;
        }

        return it->second->Start();
    }

    bool CameraManager::StopCamera(const std::uint64_t id)
    {
        const auto it = Sessions_.find(id);

        if (it == Sessions_.end())
        {
            return false;
        }

        return it->second->Stop();
    }

    std::uint64_t CameraManager::NextCameraId()
    {
        return NextId_++;
    }
}