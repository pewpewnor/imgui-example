#include "surface_lifetime.hpp"

#include <imgui-SFML.h>
#include <spdlog/spdlog.h>

#include "globals/engine_state.hpp"

SurfaceLifetime::SurfaceLifetime(const std::string& title, int width, int height)
    : title_(title), width_(width), height_(height) {}

void SurfaceLifetime::onStartup() {
    spdlog::debug("Creating window & initializing ImGui...");
    g::engine->window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode({static_cast<unsigned int>(width_), static_cast<unsigned int>(height_)}),
        title_);
    windowInitialized_ = true;

    g::engine->window->setVerticalSyncEnabled(true);
    if (!ImGui::SFML::Init(*g::engine->window)) {
        throw std::runtime_error("failed to initialize imgui-sfml");
    }
    imguiInitialized_ = true;
    spdlog::debug("Window & ImGui initialized...");

    ImGui::StyleColorsDark();
}

void SurfaceLifetime::onShutdown() {
    spdlog::debug("Closing window & shutting down ImGui...");
    if (windowInitialized_) {
        g::engine->window->close();
        windowInitialized_ = false;
    }
    if (imguiInitialized_) {
        ImGui::SFML::Shutdown();
        imguiInitialized_ = false;
    }
}
