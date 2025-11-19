#pragma once

#include <memory>

#include "app_state.h"
#include "engine/engine_state.h"
#include "workers.h"

namespace globals {

inline std::shared_ptr<AppState> appState;

inline std::shared_ptr<engine::EngineState> engineState;

inline std::shared_ptr<Workers> workers;

};
