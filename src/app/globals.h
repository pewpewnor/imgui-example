#pragma once

#include <memory>

#include "engine/engine_state.h"
#include "utils/async_worker.h"

struct AppState {
    bool showDemoWindow = false;
    AsyncWorker<std::string> sleepWorker;
    int frameCount = 0;
};

namespace globals {

inline std::shared_ptr<AppState> appState;

inline std::shared_ptr<engine::EngineState> engineState;

};
