
#include "Cameras/CameraSession.h"
#include "Streaming/ICameraStream.h"

#include <gtest/gtest.h>

#include <memory>

namespace AxiomVms::Tests
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
        Camera camera;

        auto stream = std::make_unique<FakeCameraStream>();

        const CameraSession session(camera, std::move(stream));

        EXPECT_TRUE(session.Start());
        EXPECT_EQ(camera.State, ECameraState::Connected);
        EXPECT_TRUE(session.IsRunning());
    }

    TEST(CameraSessionTests, StartFailsWhenAlreadyConnecting)
    {
        Camera camera;
        camera.State = ECameraState::Connecting;

        auto stream = std::make_unique<FakeCameraStream>();

        const CameraSession session(camera, std::move(stream));

        EXPECT_FALSE(session.Start());
        EXPECT_EQ(camera.State, ECameraState::Connecting);
    }

    TEST(CameraSessionTests, StartFailsWhenAlreadyConnected)
    {
        Camera camera;
        camera.State = ECameraState::Connected;

        auto stream = std::make_unique<FakeCameraStream>();

        const CameraSession session(camera, std::move(stream));

        EXPECT_FALSE(session.Start());
        EXPECT_EQ(camera.State, ECameraState::Connected);
    }

    TEST(CameraSessionTests, FailedStreamStartSetsCameraError)
    {
        Camera camera;

        auto stream = std::make_unique<FakeCameraStream>();
        stream->StartShouldFail = true;

        const CameraSession session(camera, std::move(stream));

        EXPECT_FALSE(session.Start());
        EXPECT_EQ(camera.State, ECameraState::Error);
        EXPECT_FALSE(session.IsRunning());
    }

    TEST(CameraSessionTests, StopDisconnectsCamera)
    {
        Camera camera;

        auto stream = std::make_unique<FakeCameraStream>();

        const CameraSession session(camera, std::move(stream));

        ASSERT_TRUE(session.Start());

        EXPECT_TRUE(session.Stop());
        EXPECT_EQ(camera.State, ECameraState::Disconnected);
        EXPECT_FALSE(session.IsRunning());
    }

    TEST(CameraSessionTests, StopFailsWhenAlreadyDisconnected)
    {
        Camera camera;
        camera.State = ECameraState::Disconnected;

        auto stream = std::make_unique<FakeCameraStream>();

        const CameraSession session(camera,std::move(stream));

        EXPECT_FALSE(session.Stop());
        EXPECT_EQ(camera.State, ECameraState::Disconnected);
    }

    TEST(CameraSessionTests, StopFailsWhenCameraStateIsNone)
    {
        Camera camera;

        auto stream = std::make_unique<FakeCameraStream>();

        const CameraSession session(camera, std::move(stream));

        EXPECT_FALSE(session.Stop());
        EXPECT_EQ(camera.State, ECameraState::None);
    }

    TEST(CameraSessionTests, FailedStreamStopSetsCameraError)
    {
        Camera camera;

        auto stream = std::make_unique<FakeCameraStream>();
        FakeCameraStream* streamPtr = stream.get();

        const CameraSession session(camera, std::move(stream));

        ASSERT_TRUE(session.Start());

        streamPtr->StopShouldFail = true;

        EXPECT_FALSE(session.Stop());
        EXPECT_EQ(camera.State, ECameraState::Error);
        EXPECT_TRUE(session.IsRunning());
    }

    TEST(CameraSessionTests, IsRunningReturnsTrueAfterStart)
    {
        Camera camera;

        auto stream = std::make_unique<FakeCameraStream>();

        const CameraSession session(camera, std::move(stream));

        ASSERT_TRUE(session.Start());

        EXPECT_TRUE(session.IsRunning());
    }

    TEST(CameraSessionTests, IsRunningReturnsFalseAfterStop)
    {
        Camera camera;

        auto stream = std::make_unique<FakeCameraStream>();

        const CameraSession session(camera, std::move(stream));

        ASSERT_TRUE(session.Start());
        ASSERT_TRUE(session.Stop());

        EXPECT_FALSE(session.IsRunning());
    }
}

