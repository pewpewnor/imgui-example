#include "engine.h"

void engine::Engine::initialize(
    const std::shared_ptr<engine::EngineState>& engineState) {
    engineState_ = engineState;
}

void engine::Engine::run() {
    assert(startupSteps_.size() > 0 &&
           "SFML and ImGui needs to have a startup");
    assert(shutdownSteps_.size() > 0 &&
           "SFML and ImGui needs to have a shutdown");
    startup();
    continouslyRenderFrames();
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

void engine::Engine::continouslyRenderFrames() {
    while (engineState_->window.isOpen() && !engineState_->stopSignal) {
        while (const auto event = engineState_->window.pollEvent()) {
            ImGui::SFML::ProcessEvent(engineState_->window, *event);

            if (event->template is<sf::Event::Closed>()) {
                engineState_->window.close();
            }
        }
        renderFrame();
    }
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
        step->onRender();
    }

    engineState_->window.clear(sf::Color::White);
    ImGui::SFML::Render(engineState_->window);
    engineState_->window.display();
}
