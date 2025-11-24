#include "engine.hpp"

#include <imgui-SFML.h>
#include <spdlog/spdlog.h>

#include <SFML/System.hpp>
#include <ranges>
#include <stdexcept>

#include "engine/engine_config.hpp"
#include "utils/assertions.hpp"
#include "utils/scopes/scope_exit.hpp"

namespace {

constexpr sf::Time fpsToTimePerFrame(int fps) { return sf::milliseconds(1000 / fps); }

}

engine::Engine::Engine(const engine::EngineConfig& engineConfig) : engineConfig(engineConfig) {}

void engine::Engine::runContinously() {
    bool expected = false;
    if (!isRunning_.compare_exchange_strong(expected, true)) {
        throw std::runtime_error("engine is already running");
    }
    ScopeExit scopeExit([this]() { shutdown(); });
    startup();
    renderFramesContinously();
}

void engine::Engine::pushStartupStep(const std::shared_ptr<engine::StartupStep>& step) {
    ASSERT(!isRunning_, "only add step while engine is not running");
    startupSteps_.push_back(step);
}

void engine::Engine::pushRenderStep(const std::shared_ptr<engine::RenderStep>& step) {
    ASSERT(!isRunning_, "only add step while engine is not running");
    renderSteps_.push_back(step);
}

void engine::Engine::pushShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step) {
    ASSERT(!isRunning_, "only add step while engine is not running");
    shutdownSteps_.push_back(step);
}

void engine::Engine::sendStopSignal() {
    spdlog::debug("Sent stop signal to engine...");
    stopSignal_ = true;
}

void engine::Engine::sendRefreshSignal() {
    spdlog::debug("Sent refresh signal to engine...");
    refreshSignal_ = true;
}

void engine::Engine::waitUntilStopped() {
    std::unique_lock<std::mutex> lock(runningMutex_);
    runningCv_.wait(lock, [this] { return !isRunning_; });
}

void engine::Engine::startup() {
    triggerTrailingRefresh_ = true;
    refreshSignal_ = true;
    spdlog::debug("Engine executing startup steps...");
    for (const std::shared_ptr<engine::StartupStep>& startupStep : startupSteps_) {
        startupStep->onStartup();
    }
}

void engine::Engine::renderFramesContinously() {
    sf::Clock clock;
    while (!stopSignal_ && window->isOpen()) {
        clock.restart();

        bool refresh = processEvents();

        sf::Time elapsed;
        sf::Time desiredDuration;
        if (refresh) {
            renderFrame();
            elapsed = clock.getElapsedTime();
            desiredDuration = fpsToTimePerFrame(engineConfig.activeFps);
        } else {
            elapsed = clock.getElapsedTime();
            desiredDuration = fpsToTimePerFrame(engineConfig.idleFps);
        }
        if (sf::Time sleepTime = desiredDuration - elapsed; sleepTime > sf::Time::Zero) {
            sf::sleep(sleepTime);
        }
    }
    spdlog::debug("Engine has stopped rendering frames");
}

bool engine::Engine::processEvents() {
    bool hasFocus = window->hasFocus();
    bool refresh = engineConfig.insistRenderMode;
    bool refreshNeedsTrailing = false;

    if (triggerTrailingRefresh_) {
        refresh = true;
        triggerTrailingRefresh_ = false;
    }

    if (pollEvents(hasFocus, refresh)) {
        refresh = true;
        refreshNeedsTrailing = true;
    }
    if (!refresh) {
        if (hasFocus && ImGui::GetIO().WantTextInput) {
            refresh = true;
        } else {
            bool expected = true;
            if (refreshSignal_.compare_exchange_strong(expected, false)) {
                refresh = true;
                refreshNeedsTrailing = true;
            }
        }
    }

    triggerTrailingRefresh_ = refreshNeedsTrailing;
    return refresh;
}

bool engine::Engine::pollEvents(bool hasFocus, bool alreadyRendering) {
    bool refresh = false;
    while (const auto event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            sendStopSignal();
            break;
        }
        /*
           do not process FocusLost event so ImGui doesn't know about it, thus we can react to
           things like hover over items even when focus is lost
         */
        if (event->is<sf::Event::FocusLost>()) {
            refresh = true;
            continue;
        }
        ImGui::SFML::ProcessEvent(*window, *event);
        if (!alreadyRendering && !refresh &&
            (hasFocus || event->is<sf::Event::FocusGained>() || event->is<sf::Event::Resized>() ||
             event->is<sf::Event::MouseButtonPressed>() ||
             event->is<sf::Event::MouseButtonReleased>() || event->is<sf::Event::MouseEntered>() ||
             event->is<sf::Event::MouseLeft>() || event->is<sf::Event::MouseMoved>() ||
             event->is<sf::Event::MouseWheelScrolled>() || event->is<sf::Event::KeyPressed>() ||
             event->is<sf::Event::KeyReleased>() || event->is<sf::Event::TextEntered>() ||
             event->is<sf::Event::JoystickButtonPressed>() ||
             event->is<sf::Event::JoystickButtonReleased>() ||
             event->is<sf::Event::JoystickMoved>() || event->is<sf::Event::JoystickConnected>() ||
             event->is<sf::Event::JoystickDisconnected>())) {
            refresh = true;
        }
    }
    return refresh;
}

void engine::Engine::renderFrame() {
    ImGui::SFML::Update(*window, deltaClock_.restart());

    for (const std::shared_ptr<engine::RenderStep>& renderStep : renderSteps_) {
        if (renderStep->shouldRender()) {
            renderStep->onRender();
        }
    }

    window->clear();
    ImGui::SFML::Render(*window);
    window->display();
}

void engine::Engine::shutdown() {
    ScopeExit scopeExit([this]() { stopRunningState(); });
    spdlog::debug("Engine executing shutdown steps...");
    for (const std::shared_ptr<engine::ShutdownStep>& shutdownStep :
         std::ranges::reverse_view(shutdownSteps_)) {
        shutdownStep->onShutdown();
    }
}

void engine::Engine::stopRunningState() {
    stopSignal_ = false;
    std::lock_guard<std::mutex> lock(runningMutex_);
    isRunning_ = false;
    runningCv_.notify_all();
}
