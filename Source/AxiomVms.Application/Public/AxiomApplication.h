#pragma once

#include <atomic>

#include "Cameras/CameraManager.h"

using namespace AxiomVms;

class AxiomApplication
{
public:

    AxiomApplication();
    ~AxiomApplication() = default;

    AxiomApplication(const AxiomApplication&) = delete;
    AxiomApplication& operator=(const AxiomApplication&) = delete;

    static void Initialize();
    static void Shutdown();
    int Run() const;
    void Stop();

private:
    CameraManager CameraManager_;

    bool Running_{false};
    std::atomic_bool StopRequested_{false};
};
