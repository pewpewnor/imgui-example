#pragma once

#include <memory>

#include "engine/engine_state.h"
#include "utils/async_worker.h"

struct AppState {
    bool showDemoWindow = false;
    int frameCount = 0;
};

class Workers {
public:
    AsyncWorker<std::string> sleepWorker;

    Workers() = default;
    Workers(const Workers&) = delete;
    Workers(Workers&&) = delete;
    Workers& operator=(const Workers&) = delete;
    Workers& operator=(Workers&&) = delete;

    ~Workers();
};

namespace globals {

inline std::shared_ptr<AppState> appState;

inline std::shared_ptr<engine::EngineState> engineState;

inline std::shared_ptr<Workers> workers;

};
