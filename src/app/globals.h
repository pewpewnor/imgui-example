#pragma once

#include <memory>

#include "app_state.h"
#include "engine/engine.h"
#include "workers.h"

namespace globals {

inline std::shared_ptr<engine::Engine> engine;

inline std::shared_ptr<AppState> appState;

inline std::shared_ptr<Workers> workers;

};
