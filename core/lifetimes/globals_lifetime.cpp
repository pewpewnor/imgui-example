#include "globals_lifetime.hpp"

#include <spdlog/spdlog.h>

#include "globals/dispatch_state.hpp"
#include "globals/ignored_futures.hpp"
#include "globals/tasks.hpp"

void GlobalsLifetime::onStartup() {
    g::dispatch = std::make_unique<g::DispatchState>();
    g::ignoredFutures = std::make_unique<g::IgnoredFutures>();
    g::tasks = std::make_unique<g::Tasks>();
    spdlog::debug("Global variables initialized");
}

void GlobalsLifetime::onShutdown() {
    spdlog::debug("Destroying global variables...");
    g::tasks.reset();
    g::ignoredFutures.reset();
    g::dispatch.reset();
    spdlog::debug("Global variables destroyed");
}
