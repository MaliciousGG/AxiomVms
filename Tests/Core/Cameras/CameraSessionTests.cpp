
#include "Cameras/CameraSession.h"
#include "Streaming/ICameraStream.h"

#include <gtest/gtest.h>

#include <memory>

namespace Axiom::Tests
{
    namespace {
        class FakeCameraStream final : public ICameraStream
        {
        public:
            bool StartShouldFail{false};
            bool StopShouldFail{false};

            bool Start() override
            {
                if (StartShouldFail || Running_)
                {
                    return false;
                }

                Running_ = true;

                return true;
            }

            bool Stop() override
            {
                if (StopShouldFail || !Running_)
                {
                    return false;
                }

                Running_ = false;

                return true;
            }

            [[nodiscard]]
            bool IsRunning() const override
            {
                return Running_;
            }

            [[nodiscard]] CameraStreamStats GetStats() const override
            {
                return CameraStreamStats{};
            };

        private:
            bool Running_{false};
        };
    }

    TEST(CameraSessionTests, StartConnectsCamera)
    {
        Camera Camera;

        auto Stream = std::make_unique<FakeCameraStream>();

        const CameraSession Session(Camera, std::move(Stream));

        EXPECT_TRUE(Session.Start());
        EXPECT_EQ(Camera.State, ECameraState::Connected);
        EXPECT_TRUE(Session.IsRunning());
    }

    TEST(CameraSessionTests, StartFailsWhenAlreadyConnecting)
    {
        Camera Camera;
        Camera.State = ECameraState::Connecting;

        auto Stream = std::make_unique<FakeCameraStream>();

        const CameraSession Session(Camera, std::move(Stream));

        EXPECT_FALSE(Session.Start());
        EXPECT_EQ(Camera.State, ECameraState::Connecting);
    }

    TEST(CameraSessionTests, StartFailsWhenAlreadyConnected)
    {
        Camera Camera;
        Camera.State = ECameraState::Connected;

        auto Stream = std::make_unique<FakeCameraStream>();

        const CameraSession Session(Camera, std::move(Stream));

        EXPECT_FALSE(Session.Start());
        EXPECT_EQ(Camera.State, ECameraState::Connected);
    }

    TEST(CameraSessionTests, FailedStreamStartSetsCameraError)
    {
        Camera Camera;

        auto Stream = std::make_unique<FakeCameraStream>();
        Stream->StartShouldFail = true;

        const CameraSession Session(Camera, std::move(Stream));

        EXPECT_FALSE(Session.Start());
        EXPECT_EQ(Camera.State, ECameraState::Error);
        EXPECT_FALSE(Session.IsRunning());
    }

    TEST(CameraSessionTests, StopDisconnectsCamera)
    {
        Camera Camera;

        auto Stream = std::make_unique<FakeCameraStream>();

        const CameraSession Session(Camera, std::move(Stream));

        ASSERT_TRUE(Session.Start());

        EXPECT_TRUE(Session.Stop());
        EXPECT_EQ(Camera.State, ECameraState::Disconnected);
        EXPECT_FALSE(Session.IsRunning());
    }

    TEST(CameraSessionTests, StopFailsWhenAlreadyDisconnected)
    {
        Camera Camera;
        Camera.State = ECameraState::Disconnected;

        auto Stream = std::make_unique<FakeCameraStream>();

        const CameraSession Session(Camera,std::move(Stream));

        EXPECT_FALSE(Session.Stop());
        EXPECT_EQ(Camera.State, ECameraState::Disconnected);
    }

    TEST(CameraSessionTests, StopFailsWhenCameraStateIsNone)
    {
        Camera Camera;

        auto Stream = std::make_unique<FakeCameraStream>();

        const CameraSession Session(Camera, std::move(Stream));

        EXPECT_FALSE(Session.Stop());
        EXPECT_EQ(Camera.State, ECameraState::None);
    }

    TEST(CameraSessionTests, FailedStreamStopSetsCameraError)
    {
        Camera Camera;

        auto Stream = std::make_unique<FakeCameraStream>();
        FakeCameraStream* StreamPtr = Stream.get();

        const CameraSession Session(Camera, std::move(Stream));

        ASSERT_TRUE(Session.Start());

        StreamPtr->StopShouldFail = true;

        EXPECT_FALSE(Session.Stop());
        EXPECT_EQ(Camera.State, ECameraState::Error);
        EXPECT_TRUE(Session.IsRunning());
    }

    TEST(CameraSessionTests, IsRunningReturnsTrueAfterStart)
    {
        Camera Camera;

        auto Stream = std::make_unique<FakeCameraStream>();

        const CameraSession Session(Camera, std::move(Stream));

        ASSERT_TRUE(Session.Start());
        EXPECT_TRUE(Session.IsRunning());
    }

    TEST(CameraSessionTests, IsRunningReturnsFalseAfterStop)
    {
        Camera Camera;

        auto Stream = std::make_unique<FakeCameraStream>();

        const CameraSession Session(Camera, std::move(Stream));

        ASSERT_TRUE(Session.Start());
        ASSERT_TRUE(Session.Stop());

        EXPECT_FALSE(Session.IsRunning());
    }
}

