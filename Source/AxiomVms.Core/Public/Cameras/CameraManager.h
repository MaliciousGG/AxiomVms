#pragma once

#include "Cameras/Camera.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace Axiom
{
    class CameraSession;
    class ICameraStreamFactory;

    class CameraManager
    {
    public:
        explicit CameraManager(std::unique_ptr<ICameraStreamFactory> InStreamFactory);

        ~CameraManager();

        std::uint64_t AddCamera(std::string InCameraName, std::string InStreamUrl);

        bool RemoveCamera(std::uint64_t OutCameraName);

        Camera* FindCamera(std::uint64_t CameraId);
        const Camera* FindCamera(std::uint64_t CameraId) const;

        Camera* FindCamera(const std::string& CameraName);
        const Camera* FindCamera(const std::string& CameraName) const;
        
        std::size_t GetCameraCount() const;

        bool StartCamera(std::uint64_t CameraId);
        bool StopCamera(std::uint64_t CameraId);

    private:
        std::uint64_t NextCameraId();

        std::unique_ptr<ICameraStreamFactory> StreamFactory_;

        std::unordered_map<std::uint64_t, Camera> Cameras_;
        std::unordered_map<std::uint64_t, std::unique_ptr<CameraSession>> Sessions_;

        std::uint64_t NextId_{1};
    };
}