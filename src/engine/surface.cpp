#include "surface.h"

#include "imgui.h"

engine::Surface::Surface(
    const std::shared_ptr<engine::EngineState>& engineState,
    const std::string& title, int width, int height, bool vsync)
    : engineState_(engineState),
      title_(title),
      width_(width),
      height_(height),
      vsync_(vsync) {}

void engine::Surface::onStartup() {
    engineState_->window =
        sf::RenderWindow(sf::VideoMode({static_cast<unsigned int>(width_),
                                        static_cast<unsigned int>(height_)}),
                         title_);
    if (vsync_) {
        engineState_->window.setVerticalSyncEnabled(true);
    } else {
        engineState_->window.setFramerateLimit(60);
    }
    if (!ImGui::SFML::Init(engineState_->window)) {
        throw std::runtime_error("failed to initialize ImGui-SFML");
    }
    ImGui::StyleColorsDark();
}

void engine::Surface::onShutdown() { ImGui::SFML::Shutdown(); }
