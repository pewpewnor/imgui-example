#pragma once

#include <memory>

#include "glfw_bindings.h"

namespace engine {

struct State {
    std::string title;
    int width = 1280;
    int height = 720;
    bool vsync = true;
    float scale = 1;
    std::shared_ptr<glfw::Window> window;
    bool stopSignal = false;
};

}
