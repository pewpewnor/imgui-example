#include "glfw_context.h"

#include <iostream>

#include "glfw.h"

namespace {

void glfwErrorCallback(int errorCode, const char* description) {
    std::cerr << "GLFW Error " << errorCode << ": " << description << std::endl;
}

}

surface::GlfwContext::GlfwContext() {
    glfw::setErrorCallback(glfwErrorCallback);
    if (!glfw::init()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

surface::GlfwContext::~GlfwContext() { glfw::terminate(); }
