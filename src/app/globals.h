#pragma once

#include <memory>

#include "engine/engine_state.h"
#include "utils/async_worker.h"

struct AppState {
    bool showDemoWindow = false;
    int frameCount = 0;
};

struct Workers {
    AsyncWorker<std::string> sleepWorker;
};

namespace globals {

inline std::shared_ptr<AppState> appState;

inline std::shared_ptr<engine::EngineState> engineState;

inline std::shared_ptr<Workers> workers;

};
