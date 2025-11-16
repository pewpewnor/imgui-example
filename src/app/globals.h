#pragma once

#include <memory>

#include "engine/engine_state.h"

struct AppState {
    bool showDemoWindow = false;
};

namespace globals {

inline auto appState = std::make_shared<AppState>();

inline auto engineState = std::make_shared<engine::EngineState>();

};
