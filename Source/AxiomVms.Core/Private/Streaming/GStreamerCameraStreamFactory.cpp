#include "Streaming/GStreamerCameraStreamFactory.h"
#include "Streaming/GStreamerCameraStream.h"

#include <memory>

namespace AxiomVms
{
    std::unique_ptr<ICameraStream> GStreamerCameraStreamFactory::CreateCameraStream(const Camera& camera)
    {
        // Create a new GStreamer camera stream and return it
        return std::make_unique<GStreamerCameraStream>(camera.StreamUrl);
    }
}