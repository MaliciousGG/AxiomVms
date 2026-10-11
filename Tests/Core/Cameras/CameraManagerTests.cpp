#include "Cameras/CameraManager.h"
#include "Streaming/ICameraStream.h"
#include "Streaming/ICameraStreamFactory.h"
#include "Streaming/NullCameraStreamFactory.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>


namespace {
    class FailingCameraStreamFactory final : public Axiom::ICameraStreamFactory
    {
    public:
        std::unique_ptr<Axiom::ICameraStream> CreateCameraStream(const Axiom::Camera& InCamera) override
        {
            static_cast<void>(InCamera);

            return nullptr;
        }
    };
}

namespace Axiom::Tests
{

    TEST(CameraManagerTests, AddCameraIncreasesCameraCount)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t CamId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        EXPECT_NE(CamId, 0);
        EXPECT_EQ(Manager.GetCameraCount(), 1);
    }

    TEST(CameraManagerTests, AddedCameraCanBeFound)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t CamId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        const Camera* Camera = Manager.FindCamera(CamId);

        ASSERT_NE(Camera, nullptr);

        EXPECT_EQ(Camera->Id, CamId);
        EXPECT_EQ(Camera->Name, "Front Entrance");
        EXPECT_EQ(Camera->StreamUrl, "rtsp://127.0.0.1/test");
    }

    TEST(CameraManagerTests, UnknownCameraCannotBeFound)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        EXPECT_EQ(Manager.FindCamera(999), nullptr);
    }

    TEST(CameraManagerTests, CameraCanBeRemoved)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t CamId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(CamId, 0);
        EXPECT_TRUE(Manager.RemoveCamera(CamId));

        EXPECT_EQ(Manager.GetCameraCount(), 0);
        EXPECT_EQ(Manager.FindCamera(CamId), nullptr);
    }

    TEST(CameraManagerTests, UnknownCameraCannotBeRemoved)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        EXPECT_FALSE(Manager.RemoveCamera(999));
    }

    TEST(CameraManagerTests, MultipleCamerasIncreaseCameraCount)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t FirstId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        const std::uint64_t SecondId = Manager.AddCamera("Parking Lot", "rtsp://127.0.0.1/test2");

        EXPECT_NE(FirstId, 0);
        EXPECT_NE(SecondId, 0);
        EXPECT_NE(FirstId, SecondId);

        EXPECT_EQ(Manager.GetCameraCount(), 2);
    }

    TEST(CameraManagerTests, DuplicateCameraCannotBeAdded)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t FirstId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");
        const std::uint64_t SecondId = Manager.AddCamera("Front Entrance","rtsp://127.0.0.1/test");

        EXPECT_EQ(FirstId, 1);
        EXPECT_EQ(SecondId, 0);

        EXPECT_EQ(Manager.GetCameraCount(), 1);
    }

    TEST(CameraManagerTests, SameNameWithDifferentUrlCanBeAdded)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t FirstId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");
        const std::uint64_t SecondId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test2");

        EXPECT_NE(FirstId, 0);
        EXPECT_NE(SecondId, 0);

        EXPECT_EQ(Manager.GetCameraCount(), 2);
    }

    TEST(CameraManagerTests, SameUrlWithDifferentNameCanBeAdded)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t FirstId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");
        const std::uint64_t SecondId = Manager.AddCamera("Front Entrance Backup", "rtsp://127.0.0.1/test");

        EXPECT_NE(FirstId, 0);
        EXPECT_NE(SecondId, 0);

        EXPECT_EQ(Manager.GetCameraCount(), 2);
    }

    TEST(CameraManagerTests, CameraCanBeStarted)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t CamId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(CamId, 0);
        EXPECT_TRUE(Manager.StartCamera(CamId));

        const Camera* Camera = Manager.FindCamera(CamId);

        ASSERT_NE(Camera, nullptr);
        EXPECT_EQ(Camera->State, ECameraState::Connected);
    }

    TEST(CameraManagerTests, CameraCannotBeStartedTwice)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t CamId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(CamId, 0);

        EXPECT_TRUE(Manager.StartCamera(CamId));
        EXPECT_FALSE(Manager.StartCamera(CamId));
    }

    TEST(CameraManagerTests, UnknownCameraCannotBeStarted)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        EXPECT_FALSE(Manager.StartCamera(999));
    }

    TEST(CameraManagerTests, StartedCameraCanBeStopped)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        const std::uint64_t CamId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(CamId, 0);
        ASSERT_TRUE(Manager.StartCamera(CamId));
        EXPECT_TRUE(Manager.StopCamera(CamId));

        const Camera* Camera = Manager.FindCamera(CamId);

        ASSERT_NE(Camera, nullptr);

        EXPECT_EQ(Camera->State, ECameraState::Disconnected);
    }

    TEST(CameraManagerTests, UnknownCameraCannotBeStopped)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());

        EXPECT_FALSE(Manager.StopCamera(999));
    }

    TEST(CameraManagerTests, RemovingCameraRemovesItsSession)
    {
        CameraManager Manager(std::make_unique<NullCameraStreamFactory>());
        const std::uint64_t CamId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        ASSERT_NE(CamId, 0);

        ASSERT_TRUE(Manager.RemoveCamera(CamId));
        EXPECT_FALSE(Manager.StartCamera(CamId));
        EXPECT_FALSE(Manager.StopCamera(CamId));
    }

    TEST(CameraManagerTests, FailedStreamCreationDoesNotAddCamera)
    {
        CameraManager Manager(std::make_unique<FailingCameraStreamFactory>());
        const std::uint64_t CamId = Manager.AddCamera("Front Entrance", "rtsp://127.0.0.1/test");

        EXPECT_EQ(CamId, 0);
        EXPECT_EQ(Manager.GetCameraCount(), 0);
    }
}