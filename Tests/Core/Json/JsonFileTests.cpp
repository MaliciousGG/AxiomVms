#include <ranges>
#include <gtest/gtest.h>

#include "CoreMinimal.h"
#include "Core/Json/JsonFile.h"

namespace Axiom::Tests
{
    TEST(JsonFileTests, JsonFileContainsExpectedData_1)
    {
        JsonFile File;
        FString Error;

        ASSERT_TRUE(File.Load("JsonFileTestData/JsonFileContainsExpectedData_1.json", Error)) << Error;

        const JsonValue& Root = File.GetRoot();

        ASSERT_TRUE(Root.IsObject()) << "Expected a JSON object.";

        struct ExpectedData
        {
            FString Key;
            FString Value;
        };

        const ExpectedData ExpectedData[] =
        {
            {
                .Key = "name",
                .Value = "Test Camera"
            }
        };

        const JsonObject& Object = Root.AsObject();

        for (const auto&[Key, Value] : ExpectedData)
        {
            const auto Entry = Object.find(Key);

            ASSERT_NE(Entry, Object.end()) << "Missing key: " << Key;

            ASSERT_TRUE(Entry->second.IsString()) << "Expected a string for key: " << Key;

            EXPECT_EQ(Entry->second.AsFString(), Value) << "Unexpected value for key: " << Key;
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
        JsonFile File;
        FString Error;

        ASSERT_TRUE(File.Load("JsonFileTestData/JsonFileContainsExpectedData_2.json", Error)) << Error;

        const JsonValue& Root = File.GetRoot();
        ASSERT_TRUE(Root.IsObject()) << "Expected a JSON object.";

        const JsonObject& RootObject = Root.AsObject();
        ASSERT_TRUE(RootObject.contains("Cameras")) << "Missing Cameras.";

        const JsonValue& CameraValues = RootObject.at("Cameras");
        ASSERT_TRUE(CameraValues.IsObject()) << "Cameras must be an object.";

        const TMap<FString, CameraData> ExpectedCameras =
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

        const JsonObject& Cameras = CameraValues.AsObject();
        EXPECT_EQ(Cameras.size(), ExpectedCameras.size());

        for (const auto& [CameraId, Expected] : ExpectedCameras)
        {
            SCOPED_TRACE("Camera: " + CameraId);

            ASSERT_TRUE(Cameras.contains(CameraId)) << "Missing Camera.";

            const JsonValue& cameraValue = Cameras.at(CameraId);
            ASSERT_TRUE(cameraValue.IsObject());

            const JsonObject& Camera = cameraValue.AsObject();

            const TMap<FString, FString> ExpectedStrings =
            {
                {"Name", Expected.Name},
                {"Url", Expected.Url},
                {"VideoFormat", Expected.VideoFormat}
            };

            for (const auto& [Key, ExpectedValue] : ExpectedStrings)
            {
                SCOPED_TRACE("Field: " + Key);

                ASSERT_TRUE(Camera.contains(Key)) << "Missing field.";
                ASSERT_TRUE(Camera.at(Key).IsString()) << "Expected a string.";

                EXPECT_EQ(Camera.at(Key).AsFString(), ExpectedValue);
            }

            ASSERT_TRUE(Camera.contains("VideoResolution"));
            ASSERT_TRUE(Camera.at("VideoResolution").IsObject());

            const JsonObject& Resolution = Camera.at("VideoResolution").AsObject();

            ASSERT_TRUE(Resolution.contains("Width"));
            ASSERT_TRUE(Resolution.contains("Height"));
            ASSERT_TRUE(Resolution.at("Width").IsNumber());
            ASSERT_TRUE(Resolution.at("Height").IsNumber());

            EXPECT_DOUBLE_EQ(Resolution.at("Width").AsFloat64(), Expected.Resolution.Width);

            EXPECT_DOUBLE_EQ(Resolution.at("Height").AsFloat64(), Expected.Resolution.Height);
        }
    }

    TEST(JsonFileTests, JsonFileContainsExpectedData_2_StructAssign)
    {
        JsonFile File;
        FString Error;

        ASSERT_TRUE(File.Load("JsonFileTestData/JsonFileContainsExpectedData_2.json", Error)) << Error;

        const JsonValue& Root = File.GetRoot();
        const JsonObject& RootObject = Root.AsObject();
        const JsonValue& CameraValues = RootObject.at("Cameras");
        const JsonObject& Cameras = CameraValues.AsObject();

        TMap<FString, CameraData> AssignedCameras;

        for (const auto& [CameraId, CameraValue] : Cameras)
        {
            ASSERT_TRUE(CameraValue.IsObject());
            const JsonObject& Camera = CameraValue.AsObject();

            const JsonObject& Resolution = Camera.at("VideoResolution").AsObject();

            CameraData CameraData{};
            CameraData.Name = Camera.at("Name").AsFString();
            CameraData.Url = Camera.at("Url").AsFString();
            CameraData.VideoFormat = Camera.at("VideoFormat").AsFString();
            CameraData.Resolution.Width = static_cast<int>(Resolution.at("Width").AsFloat64());
            CameraData.Resolution.Height = static_cast<int>(Resolution.at("Height").AsFloat64());

            AssignedCameras.emplace(CameraId, CameraData);
        }

        ASSERT_TRUE(AssignedCameras.contains("front_entrance"));

        {
            const auto&[Name, Url, VideoFormat, Resolution] = AssignedCameras.at("front_entrance");

            EXPECT_EQ(Name, "Front Entrance");
            EXPECT_EQ(Url, "rtsp://192.168.1.100:554/user=admin&password=&channel=1&stream=0.sdp");
            EXPECT_EQ(VideoFormat, "H264");
            EXPECT_EQ(Resolution.Width, 1280);
            EXPECT_EQ(Resolution.Height, 720);
        }

        {
            const auto&[Name, Url, VideoFormat, Resolution] = AssignedCameras.at("back_entrance");

            EXPECT_EQ(Name, "Back Entrance");
            EXPECT_EQ(Url, "rtsp://192.168.1.100:554/user=admin&password=&channel=2&stream=0.sdp");
            EXPECT_EQ(VideoFormat, "H264");
            EXPECT_EQ(Resolution.Width, 1280);
            EXPECT_EQ(Resolution.Height, 720);
        }
    }
}
