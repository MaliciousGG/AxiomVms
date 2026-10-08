#include <ranges>
#include <gtest/gtest.h>

#include "CoreMinimal.h"
#include "Core/Json/JsonFile.h"

namespace AxiomVms::Tests
{
    TEST(JsonFileTests, JsonFileContainsExpectedData_1)
    {
        JsonFile file;
        FString error;

        ASSERT_TRUE(file.Load("JsonFileTestData/JsonFileContainsExpectedData_1.json", error)) << error;

        const JsonValue& root = file.GetRoot();

        ASSERT_TRUE(root.IsObject()) << "Expected a JSON object.";

        struct ExpectedData
        {
            FString Key;
            FString Value;
        };

        const ExpectedData expectedData[] =
        {
            {
                .Key = "name",
                .Value = "Test Camera"
            }
        };

        const JsonObject& object = root.AsObject();

        for (const auto&[Key, Value] : expectedData)
        {
            const auto entry = object.find(Key);

            ASSERT_NE(entry, object.end()) << "Missing key: " << Key;

            ASSERT_TRUE(entry->second.IsString()) << "Expected a string for key: " << Key;

            EXPECT_EQ(entry->second.AsFString(), Value) << "Unexpected value for key: " << Key;
        }
    }

    namespace
    {
        struct VideoResolution
        {
            int Width;
            int Height;
        };

        struct CameraData
        {
            FString Name;
            FString Url;
            FString VideoFormat;
            VideoResolution Resolution;
        };
    }

    TEST(JsonFileTests, JsonFileContainsExpectedData_2)
    {
        JsonFile file;
        FString error;

        ASSERT_TRUE(file.Load("JsonFileTestData/JsonFileContainsExpectedData_2.json", error)) << error;

        const JsonValue& root = file.GetRoot();
        ASSERT_TRUE(root.IsObject()) << "Expected a JSON object.";

        const JsonObject& rootObject = root.AsObject();
        ASSERT_TRUE(rootObject.contains("Cameras")) << "Missing Cameras.";

        const JsonValue& camerasValue = rootObject.at("Cameras");
        ASSERT_TRUE(camerasValue.IsObject()) << "Cameras must be an object.";

        const TMap<FString, CameraData> expectedCameras =
        {
            {
                "front_entrance",
                {
                    .Name = "Front Entrance",
                    .Url = "rtsp://192.168.1.100:554/user=admin&password=&channel=1&stream=0.sdp",
                    .VideoFormat = "H264",
                    .Resolution = {
                        .Width = 1280,
                        .Height = 720
                    }
                }
            },
            {
                "back_entrance",
                {
                    .Name = "Back Entrance",
                    .Url = "rtsp://192.168.1.100:554/user=admin&password=&channel=2&stream=0.sdp",
                    .VideoFormat = "H264",
                    .Resolution = {
                        .Width = 1280,
                        .Height = 720
                    }
                }
            }
        };

        const JsonObject& cameras = camerasValue.AsObject();
        EXPECT_EQ(cameras.size(), expectedCameras.size());

        for (const auto& [cameraId, expected] : expectedCameras)
        {
            SCOPED_TRACE("Camera: " + cameraId);

            ASSERT_TRUE(cameras.contains(cameraId)) << "Missing camera.";

            const JsonValue& cameraValue = cameras.at(cameraId);
            ASSERT_TRUE(cameraValue.IsObject());

            const JsonObject& camera = cameraValue.AsObject();

            const TMap<FString, FString> expectedStrings =
            {
                {"Name", expected.Name},
                {"Url", expected.Url},
                {"VideoFormat", expected.VideoFormat}
            };

            for (const auto& [key, expectedValue] : expectedStrings)
            {
                SCOPED_TRACE("Field: " + key);

                ASSERT_TRUE(camera.contains(key)) << "Missing field.";
                ASSERT_TRUE(camera.at(key).IsString()) << "Expected a string.";

                EXPECT_EQ(camera.at(key).AsFString(), expectedValue);
            }

            ASSERT_TRUE(camera.contains("VideoResolution"));
            ASSERT_TRUE(camera.at("VideoResolution").IsObject());

            const JsonObject& resolution = camera.at("VideoResolution").AsObject();

            ASSERT_TRUE(resolution.contains("Width"));
            ASSERT_TRUE(resolution.contains("Height"));
            ASSERT_TRUE(resolution.at("Width").IsNumber());
            ASSERT_TRUE(resolution.at("Height").IsNumber());

            EXPECT_DOUBLE_EQ(resolution.at("Width").AsFloat64(), expected.Resolution.Width);

            EXPECT_DOUBLE_EQ(resolution.at("Height").AsFloat64(), expected.Resolution.Height);
        }
    }

    TEST(JsonFileTests, JsonFileContainsExpectedData_2_StructAssign)
    {
        JsonFile file;
        FString error;

        ASSERT_TRUE(file.Load("JsonFileTestData/JsonFileContainsExpectedData_2.json", error)) << error;

        const JsonValue& root = file.GetRoot();
        const JsonObject& rootObject = root.AsObject();
        const JsonValue& camerasValue = rootObject.at("Cameras");
        const JsonObject& cameras = camerasValue.AsObject();

        TMap<FString, CameraData> assignedCameras;

        for (const auto& [cameraId, cameraValue] : cameras)
        {
            ASSERT_TRUE(cameraValue.IsObject());
            const JsonObject& camera = cameraValue.AsObject();

            const JsonObject& resolution = camera.at("VideoResolution").AsObject();

            CameraData cameraData{};
            cameraData.Name = camera.at("Name").AsFString();
            cameraData.Url = camera.at("Url").AsFString();
            cameraData.VideoFormat = camera.at("VideoFormat").AsFString();
            cameraData.Resolution.Width = static_cast<int>(resolution.at("Width").AsFloat64());
            cameraData.Resolution.Height = static_cast<int>(resolution.at("Height").AsFloat64());

            assignedCameras.emplace(cameraId, cameraData);
        }

        ASSERT_TRUE(assignedCameras.contains("front_entrance"));

        {
            const auto&[Name, Url, VideoFormat, Resolution] = assignedCameras.at("front_entrance");

            EXPECT_EQ(Name, "Front Entrance");
            EXPECT_EQ(Url, "rtsp://192.168.1.100:554/user=admin&password=&channel=1&stream=0.sdp");
            EXPECT_EQ(VideoFormat, "H264");
            EXPECT_EQ(Resolution.Width, 1280);
            EXPECT_EQ(Resolution.Height, 720);
        }

        {
            const auto&[Name, Url, VideoFormat, Resolution] = assignedCameras.at("back_entrance");

            EXPECT_EQ(Name, "Back Entrance");
            EXPECT_EQ(Url, "rtsp://192.168.1.100:554/user=admin&password=&channel=2&stream=0.sdp");
            EXPECT_EQ(VideoFormat, "H264");
            EXPECT_EQ(Resolution.Width, 1280);
            EXPECT_EQ(Resolution.Height, 720);
        }
    }
}
