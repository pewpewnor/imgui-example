#pragma once

#include <memory>

namespace globals {

struct AppState {
    bool showDemoWindow = false;
    bool showWindow2 = false;
    int frameCount = 0;
};

inline std::unique_ptr<AppState> appState;

}
