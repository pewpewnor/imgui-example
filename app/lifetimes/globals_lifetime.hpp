#pragma once

#include "engine/shutdown_step.hpp"
#include "engine/startup_step.hpp"

class GlobalsLifetime : public engine::StartupStep, public engine::ShutdownStep {
public:
    void onStartup() override;

    void onShutdown() override;
};
