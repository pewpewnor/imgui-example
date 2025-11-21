#pragma once

#include <atomic>
class Application {
public:
    static std::atomic<bool> stopPrematurely;

    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application();

    static void start();

    static void requestStop();
};
