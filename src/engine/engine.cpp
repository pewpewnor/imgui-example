#include "engine.h"

#include <SFML/System.hpp>

#include "SFML/System/Time.hpp"

namespace {
constexpr sf::Time timePerFrameMillisecFromFps(int fps) {
    return sf::milliseconds(1000 / fps);
}
}

void engine::Engine::initialize(
    const std::shared_ptr<engine::EngineState>& engineState) {
    engineState_ = engineState;
}

void engine::Engine::runContinously() {
    assert(startupSteps_.size() > 0 &&
           "SFML and ImGui needs to have a startup");
    assert(shutdownSteps_.size() > 0 &&
           "SFML and ImGui needs to have a shutdown");
    startup();
    renderFramesContinously();
    shutdown();
}

void engine::Engine::pushStartupStep(
    const std::shared_ptr<engine::StartupStep>& step) {
    startupSteps_.push_back(step);
}

void engine::Engine::pushRenderStep(
    const std::shared_ptr<engine::RenderStep>& step) {
    renderSteps_.push_back(step);
}

void engine::Engine::pushShutdownStep(
    const std::shared_ptr<engine::ShutdownStep>& step) {
    shutdownSteps_.push_back(step);
}

void engine::Engine::startup() {
    for (const auto& step : startupSteps_) {
        step->onStartup();
    }
    startupSteps_.clear();
}

void engine::Engine::renderFramesContinously() {
    sf::Time idleSleepTime = timePerFrameMillisecFromFps(20);

    while (engineState_->window.isOpen() && !engineState_->stopSignal) {
        bool hasFocus = engineState_->window.hasFocus();
        bool refresh = false;

        while (const auto event = engineState_->window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                engineState_->stopSignal = true;
                break;
            }
            if (event->is<sf::Event::FocusLost>()) {
                continue;
            }
            ImGui::SFML::ProcessEvent(engineState_->window, *event);

            if (hasFocus || event->is<sf::Event::FocusGained>() ||
                event->is<sf::Event::Resized>() ||
                event->is<sf::Event::MouseButtonPressed>() ||
                event->is<sf::Event::MouseEntered>() ||
                event->is<sf::Event::MouseLeft>() ||
                event->is<sf::Event::MouseMoved>() ||
                event->is<sf::Event::MouseWheelScrolled>()) {
                refresh = true;
            }
        }
        if (engineState_->stopSignal) {
            break;
        }

        if (hasFocus && ImGui::GetIO().WantTextInput) {
            refresh = true;
        }

        if (engineState_->refreshSignal) {
            refresh = true;
            engineState_->refreshSignal = false;
        }

        if (refresh) {
            renderFrame();
        } else {
            sf::sleep(idleSleepTime);
        }
    }

    engineState_->window.close();
    renderSteps_.clear();
}

void engine::Engine::shutdown() {
    for (const auto& step : shutdownSteps_) {
        step->onShutdown();
    }
    shutdownSteps_.clear();
    engineState_.reset();
}

void engine::Engine::renderFrame() {
    ImGui::SFML::Update(engineState_->window, deltaClock_.restart());

    for (const auto& step : renderSteps_) {
        if (step->shouldRender()) {
            step->onRender();
        }
    }

    engineState_->window.clear(sf::Color::White);
    ImGui::SFML::Render(engineState_->window);
    engineState_->window.display();
}
