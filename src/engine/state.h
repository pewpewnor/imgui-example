#pragma once

#include <memory>

#include "glfw_bindings.h"

namespace engine {

struct State {
    std::shared_ptr<glfw::Window> window;
    bool stopSignal = false;
};

}
