#pragma once

#include <memory>

namespace g {

struct DispatchState {
    bool showImguiDemoWindow = false;
};

inline std::unique_ptr<g::DispatchState> dispatch;

}
