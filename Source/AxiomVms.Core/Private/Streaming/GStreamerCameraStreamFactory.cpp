#include "Streaming/GStreamerCameraStreamFactory.h"
#include "Streaming/GStreamerCameraStream.h"

#include <memory>

namespace Axiom
{
    std::unique_ptr<ICameraStream> GStreamerCameraStreamFactory::CreateCameraStream(const Camera& InCamera)
    {
        // Create a new GStreamer camera stream and return it
        return std::make_unique<GStreamerCameraStream>(InCamera.StreamUrl);
    }
}