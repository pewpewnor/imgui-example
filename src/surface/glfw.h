#pragma once

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include <string>

namespace glfw {

using Window = GLFWwindow;

bool init();

glfw::Window* createWindow(int width, int height, const std::string& title);

GLFWmonitor* getPrimaryMonitor();

void setErrorCallback(GLFWerrorfun callback);

void terminate();

}
