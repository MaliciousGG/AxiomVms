#include "Streaming/NullCameraStreamFactory.h"
#include "Streaming/NullCameraStream.h"

#include <memory>

namespace Axiom
{
    std::unique_ptr<ICameraStream> NullCameraStreamFactory::CreateCameraStream(const Camera& InCamera)
    {
        static_cast<void>(InCamera);

        return std::make_unique<NullCameraStream>();
    }
}