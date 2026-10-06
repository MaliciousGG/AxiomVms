#include "AxiomApplication.h"

#include <thread>

#include "Core/Logging/ConsoleLogger.h"
#include "Core/Logging/Log.h"
#include "Streaming/GStreamerCameraStreamFactory.h"


AxiomApplication::AxiomApplication()
    : CameraManager_(std::make_unique<GStreamerCameraStreamFactory>())
{
    Initialize();
}

void AxiomApplication::Initialize()
{
    Log::Configure(std::make_unique<ConsoleLogger>());

    AXIOM_LOG_INFO("AxiomVms Application is initializing.");
}

void AxiomApplication::Shutdown()
{
}

int AxiomApplication::Run() const
{
    // Start configured cameras here.
    // If startup fails, clean up and return EXIT_FAILURE.

    while (!StopRequested_.load())
    {

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return EXIT_SUCCESS;
}

void AxiomApplication::Stop()
{
    StopRequested_.store(true);
}
