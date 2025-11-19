#include <csignal>

#include "application.h"
#include "spdlog/spdlog.h"

namespace {

void handleStopSignal(int signal) {
    spdlog::info("Signal {} received.", signal);
    Application::stop();
}

}

int main() {
    std::signal(SIGINT, handleStopSignal);
    std::signal(SIGTERM, handleStopSignal);

    try {
        spdlog::info("Setting up application...");
        Application app;
        spdlog::info("Running application...");
        Application::start();
    } catch (const std::exception& error) {
        spdlog::error("Error occurred: {}", error.what());
        return 1;
    } catch (...) {
        spdlog::error("Unknown fatal error occurred");
        return 1;
    }

    return 0;
}
