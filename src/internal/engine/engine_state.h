#pragma once

#include <atomic>

#include "SFML/Graphics/RenderWindow.hpp"

namespace engine {

class EngineState {
public:
    sf::RenderWindow window;
    std::atomic<bool> stopSignal;
    std::atomic<bool> refreshSignal;

    void sendStopSignal() { stopSignal = true; }

    void sendRefreshSignal() { refreshSignal = true; }
};

}
