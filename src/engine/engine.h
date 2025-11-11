#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cassert>
#include <memory>
#include <vector>

#include "glfw_bindings.h"
#include "render_layer.h"
#include "rigging.h"
#include "shutdown_layer.h"
#include "startup_layer.h"

namespace engine {

class Engine {
public:
    void run() {
        startup();
        while (!shouldStop()) {
            glfw::pollEvents();
            if (glfw::windowAttributeIsError(rigging_->window)) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
            render();
        }
        shutdown();
    }

    void addStartupLayer(const std::shared_ptr<engine::StartupLayer>& layer) {
        startupLayers_.push_back(layer);
    }

    void addRenderLayer(const std::shared_ptr<engine::RenderLayer>& layer) {
        renderLayers_.push_back(layer);
    }

    void addShutdownLayer(const std::shared_ptr<engine::ShutdownLayer>& layer) {
        shutdownLayers_.push_back(layer);
    }

    void requestStop() { rigging_->stopSignal = true; }

private:
    std::vector<std::shared_ptr<engine::StartupLayer>> startupLayers_;
    std::vector<std::shared_ptr<engine::RenderLayer>> renderLayers_;
    std::vector<std::shared_ptr<engine::ShutdownLayer>> shutdownLayers_;
    std::shared_ptr<engine::Rigging> rigging_ =
        std::make_shared<engine::Rigging>();

    void startup() {
        assert(startupLayers_.size() > 0 &&
               "GLFW and ImGui needs to be initialized");

        for (const auto& layer : startupLayers_) {
            layer->execute(rigging_);
        }
    }

    void render() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (const auto& layer : renderLayers_) {
            layer->render(rigging_);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfw::swapBuffers(rigging_->window);
    }

    void shutdown() {
        renderLayers_.clear();
        for (const auto& layer : shutdownLayers_) {
            layer->execute(rigging_);
        }
        shutdownLayers_.clear();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        rigging_.reset();
        glfw::terminate();
    }

    bool shouldStop() {
        return glfw::windowShouldClose(rigging_->window) ||
               rigging_->stopSignal;
    }
};

}
