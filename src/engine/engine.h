#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cassert>
#include <memory>
#include <vector>

#include "glfw_bindings.h"
#include "render_step.h"
#include "shutdown_step.h"
#include "startup_step.h"
#include "state.h"

namespace engine {

class Engine {
public:
    void run() {
        assert(startupSteps_.size() > 0 &&
               "GLFW and ImGui needs to have a startup");
        assert(shutdownSteps_.size() > 0 &&
               "GLFW and ImGui needs to have a shutdown");

        startup();
        continouslyRenderFrames();
        shutdown();
    }

    void addStartupStep(const std::shared_ptr<engine::StartupStep>& step) {
        startupSteps_.push_back(step);
    }

    void addRenderStep(const std::shared_ptr<engine::RenderStep>& step) {
        renderSteps_.push_back(step);
    }

    void addShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step) {
        shutdownSteps_.push_back(step);
    }

    void requestStop() { engineState_->stopSignal = true; }

private:
    std::vector<std::shared_ptr<engine::StartupStep>> startupSteps_;
    std::vector<std::shared_ptr<engine::RenderStep>> renderSteps_;
    std::vector<std::shared_ptr<engine::ShutdownStep>> shutdownSteps_;
    std::shared_ptr<engine::State> engineState_ =
        std::make_shared<engine::State>();

    void startup() {
        for (const auto& step : startupSteps_) {
            step->onStartup(engineState_);
        }
        startupSteps_.clear();
    }

    void continouslyRenderFrames() {
        while (!glfw::windowShouldClose(engineState_->window) &&
               !engineState_->stopSignal) {
            glfw::pollEvents();
            if (glfw::windowAttributeIsError(engineState_->window)) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
            renderFrame();
        }
        renderSteps_.clear();
    }

    void shutdown() {
        for (const auto& step : shutdownSteps_) {
            step->onShutdown(engineState_);
        }
        shutdownSteps_.clear();
        engineState_.reset();
    }

    void renderFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (const auto& step : renderSteps_) {
            step->onRender(engineState_);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfw::swapBuffers(engineState_->window);
    }
};

}
