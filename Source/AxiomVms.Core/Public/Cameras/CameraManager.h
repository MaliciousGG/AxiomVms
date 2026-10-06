#pragma once

#include "Cameras/Camera.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace AxiomVms
{
    class CameraSession;
    class ICameraStreamFactory;

    class CameraManager
    {
    public:
        explicit CameraManager(std::unique_ptr<ICameraStreamFactory> streamFactory);

        ~CameraManager();

        std::uint64_t AddCamera(std::string name, std::string streamUrl);

        bool RemoveCamera(std::uint64_t id);

        Camera* FindCamera(std::uint64_t id);
        const Camera* FindCamera(std::uint64_t id) const;

        Camera* FindCamera(const std::string& name);
        const Camera* FindCamera(const std::string& name) const;
        
        std::size_t GetCameraCount() const;

        bool StartCamera(std::uint64_t id);
        bool StopCamera(std::uint64_t id);

    private:
        std::uint64_t NextCameraId();

        std::unique_ptr<ICameraStreamFactory> StreamFactory_;

        std::unordered_map<std::uint64_t, Camera> Cameras_;
        std::unordered_map<std::uint64_t, std::unique_ptr<CameraSession>> Sessions_;

        std::uint64_t NextId_{1};
    };
}