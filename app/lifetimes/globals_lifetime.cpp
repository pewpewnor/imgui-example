#include "globals_lifetime.hpp"

#include "globals/app_state.hpp"
#include "globals/tasks.hpp"

void GlobalsLifetime::onStartup() {
    globals::appState = std::make_unique<globals::AppState>();
    globals::ignoredFutures = std::make_unique<globals::IgnoredFutures>();
    globals::tasks = std::make_unique<globals::Tasks>();
    spdlog::debug("Global variables initialized");
}

void GlobalsLifetime::onShutdown() {
    spdlog::debug("Destroying global variables...");
    globals::tasks.reset();
    globals::ignoredFutures.reset();
    globals::appState.reset();
    spdlog::debug("Global variables destroyed");
}
