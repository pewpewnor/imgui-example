#pragma once

#include <imgui.h>

#include <memory>

#include "surface/glfw_context.h"
#include "surface/glfw_window.h"
#include "surface/imgui_context.h"

class Application {
public:
    Application();
    ~Application();

    // Delete copy and move constructors/operators
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    bool initialize();
    void run();
    void shutdown();

private:
    // Initialization helpers
    bool initializeImGui();

    // Main loop functions
    void handleInput();
    void renderFrame();
    void renderUI();

    // Context and window management
    std::unique_ptr<surface::GlfwContext> glfw_context_;
    std::unique_ptr<surface::GlfwWindow> window_;
    std::unique_ptr<surface::ImGuiContext> imgui_context_;

    // Application state
    bool is_initialized_;
    float scale_;

    bool show_demo_window_;
    ImVec4 clear_color_;
    int counter_;
    float slider_value_;
};
