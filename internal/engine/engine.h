#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

#include "render_step.h"
#include "shutdown_step.h"
#include "startup_step.h"

namespace engine {

class Engine {
public:
    std::shared_ptr<sf::RenderWindow> window;

    void runContinously();

    void pushStartupStep(const std::shared_ptr<engine::StartupStep>& step);

    void pushRenderStep(const std::shared_ptr<engine::RenderStep>& step);

    void pushShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step);

    void sendStopSignal();

    void sendRefreshSignal();

    void waitUntilStopped();

private:
    std::atomic<bool> isRunning_ = false;
    std::mutex runningMutex_;
    std::condition_variable runningCv_;
    bool imguiInitialized_ = false;
    std::atomic<bool> stopSignal_ = false;
    std::atomic<bool> refreshSignal_ = false;

    sf::Clock deltaClock_;
    std::vector<std::shared_ptr<engine::StartupStep>> startupSteps_;
    std::vector<std::shared_ptr<engine::RenderStep>> renderSteps_;
    std::vector<std::shared_ptr<engine::ShutdownStep>> shutdownSteps_;

    void startup();

    void renderFramesContinously();

    bool processEvents();

    void renderFrame();

    void shutdown();

    void stopRunningState();
};
}
