#include "Streaming/NullCameraStreamFactory.h"
#include "Streaming/NullCameraStream.h"

#include <memory>

namespace AxiomVms
{
    std::unique_ptr<ICameraStream> NullCameraStreamFactory::CreateCameraStream(const Camera& camera)
    {
        static_cast<void>(camera);

        return std::make_unique<NullCameraStream>();
    }
}