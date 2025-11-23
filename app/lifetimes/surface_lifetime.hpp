#pragma once

#include <string>

#include "engine/shutdown_step.hpp"
#include "engine/startup_step.hpp"

class SurfaceLifetime : public engine::StartupStep, public engine::ShutdownStep {
public:
    SurfaceLifetime(const std::string& title, int width, int height);

    void onStartup() override;

    void onShutdown() override;

private:
    std::string title_;
    int width_;
    int height_;

    bool windowInitialized_ = false;
    bool imguiInitialized_ = false;
};
