#pragma once

class Application {
public:
    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application();

    static void start();

    static void requestStop();

private:
    static void pushLifetimeSteps();

    static void pushRenderSteps();
};
