#include "Cameras/CameraManager.h"
#include "Streaming/ICameraStream.h"
#include "Streaming/ICameraStreamFactory.h"
#include "Streaming/NullCameraStreamFactory.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>


namespace {
    class FailingCameraStreamFactory final : public AxiomVms::ICameraStreamFactory
    {
    public:
        std::unique_ptr<AxiomVms::ICameraStream> CreateCameraStream(const AxiomVms::Camera& camera) override
        {
            static_cast<void>(camera);

            return nullptr;
        }
    };
}

namespace AxiomVms::Tests
{

    TEST(CameraManagerTests, AddCameraIncreasesCameraCount)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t id = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        EXPECT_NE(id, 0);
        EXPECT_EQ(manager.GetCameraCount(), 1);
    }

    TEST(CameraManagerTests, AddedCameraCanBeFound)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t id = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        const Camera* camera = manager.FindCamera(id);

        ASSERT_NE(camera, nullptr);

        EXPECT_EQ(camera->Id, id);
        EXPECT_EQ(camera->Name, "Front Entrance");
        EXPECT_EQ(camera->StreamUrl, "rtsp://127.0.0.1/test");
    }

    TEST(CameraManagerTests, UnknownCameraCannotBeFound)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        EXPECT_EQ(manager.FindCamera(999), nullptr);
    }

    TEST(CameraManagerTests, CameraCanBeRemoved)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t id = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(id, 0);
        EXPECT_TRUE(manager.RemoveCamera(id));

        EXPECT_EQ(manager.GetCameraCount(), 0);
        EXPECT_EQ(manager.FindCamera(id), nullptr);
    }

    TEST(CameraManagerTests, UnknownCameraCannotBeRemoved)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        EXPECT_FALSE(manager.RemoveCamera(999));
    }

    TEST(CameraManagerTests, MultipleCamerasIncreaseCameraCount)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t firstId = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        const std::uint64_t secondId = manager.AddCamera("Parking Lot", "rtsp://127.0.0.1/test2");

        EXPECT_NE(firstId, 0);
        EXPECT_NE(secondId, 0);
        EXPECT_NE(firstId, secondId);

        EXPECT_EQ(manager.GetCameraCount(), 2);
    }

    TEST(CameraManagerTests, DuplicateCameraCannotBeAdded)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t firstId = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");
        const std::uint64_t secondId = manager.AddCamera("Front Entrance","rtsp://127.0.0.1/test");

        EXPECT_EQ(firstId, 1);
        EXPECT_EQ(secondId, 0);

        EXPECT_EQ(manager.GetCameraCount(), 1);
    }

    TEST(CameraManagerTests, SameNameWithDifferentUrlCanBeAdded)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t firstId = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");
        const std::uint64_t secondId = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test2");

        EXPECT_NE(firstId, 0);
        EXPECT_NE(secondId, 0);

        EXPECT_EQ(manager.GetCameraCount(), 2);
    }

    TEST(CameraManagerTests, SameUrlWithDifferentNameCanBeAdded)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t firstId = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");
        const std::uint64_t secondId = manager.AddCamera("Front Entrance Backup", "rtsp://127.0.0.1/test");

        EXPECT_NE(firstId, 0);
        EXPECT_NE(secondId, 0);

        EXPECT_EQ(manager.GetCameraCount(), 2);
    }

    TEST(CameraManagerTests, CameraCanBeStarted)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t id = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(id, 0);
        EXPECT_TRUE(manager.StartCamera(id));

        const Camera* camera = manager.FindCamera(id);

        ASSERT_NE(camera, nullptr);
        EXPECT_EQ(camera->State, ECameraState::Connected);
    }

    TEST(CameraManagerTests, CameraCannotBeStartedTwice)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t id = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(id, 0);

        EXPECT_TRUE(manager.StartCamera(id));
        EXPECT_FALSE(manager.StartCamera(id));
    }

    TEST(CameraManagerTests, UnknownCameraCannotBeStarted)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        EXPECT_FALSE(manager.StartCamera(999));
    }

    TEST(CameraManagerTests, StartedCameraCanBeStopped)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t id = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(id, 0);
        ASSERT_TRUE(manager.StartCamera(id));
        EXPECT_TRUE(manager.StopCamera(id));

        const Camera* camera = manager.FindCamera(id);

        ASSERT_NE(camera, nullptr);

        EXPECT_EQ(camera->State, ECameraState::Disconnected);
    }

    TEST(CameraManagerTests, UnknownCameraCannotBeStopped)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());

        EXPECT_FALSE(manager.StopCamera(999));
    }

    TEST(CameraManagerTests, RemovingCameraRemovesItsSession)
    {
        CameraManager manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t id = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(id, 0);

        ASSERT_TRUE(manager.RemoveCamera(id));
        EXPECT_FALSE(manager.StartCamera(id));
        EXPECT_FALSE(manager.StopCamera(id));
    }

    TEST(CameraManagerTests, FailedStreamCreationDoesNotAddCamera)
    {
        CameraManager manager(std::make_unique<FailingCameraStreamFactory>());
        const std::uint64_t id = manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        EXPECT_EQ(id, 0);
        EXPECT_EQ(manager.GetCameraCount(), 0);
    }
}