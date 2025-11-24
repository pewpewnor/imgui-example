#include "application.hpp"

#include <spdlog/spdlog.h>

#include <memory>

#include "engine/engine.hpp"
#include "engine/engine_config.hpp"
#include "globals/engine_state.hpp"
#include "keys/debug_key_handler.hpp"
#include "layers/main_window.hpp"
#include "lifetimes/globals_lifetime.hpp"
#include "lifetimes/surface_lifetime.hpp"
#include "utils/assertions.hpp"

Application::Application() {
    g::engine = std::make_unique<engine::Engine>(engine::EngineConfig{false, 70, 30});

    pushLifetimeSteps();
    pushRenderSteps();
}

Application::~Application() { g::engine.reset(); }

void Application::start() {
    ASSERT(g::engine, "only execute application with engine existing");
    spdlog::info("Running application...");
    g::engine->runContinously();
    spdlog::info("Application stopped");
}

void Application::requestStop() {
    if (g::engine) {
        g::engine->sendStopSignal();
    }
}

void Application::pushLifetimeSteps() {
    auto globalsLifetime = std::make_shared<GlobalsLifetime>();
    g::engine->pushStartupStep(globalsLifetime);
    g::engine->pushShutdownStep(globalsLifetime);

    auto surfaceLifetime = std::make_shared<SurfaceLifetime>("Tuesdays", 1280, 720);
    g::engine->pushStartupStep(surfaceLifetime);
    g::engine->pushShutdownStep(surfaceLifetime);

#ifndef NDEBUG
    g::engine->pushRenderStep(std::make_shared<DebugKeyHandler>());
#endif
}

void Application::pushRenderSteps() { g::engine->pushRenderStep(std::make_shared<MainWindow>()); }
