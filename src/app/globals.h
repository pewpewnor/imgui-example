#pragma once

#include <memory>

#include "engine/engine_state.h"
struct AppState {
    bool showDemoWindow = false;
};

inline AppState appState;

inline auto engineState = std::make_shared<engine::EngineState>();
