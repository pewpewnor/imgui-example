#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "engine/render_step.hpp"
#include "utils/key_press_detector.hpp"

class DebugKeyHandler : public engine::RenderStep {
public:
    void onRender() override;

private:
    KeyPressDetector f1Key_{sf::Keyboard::Key::F1};
    KeyPressDetector f2Key_{sf::Keyboard::Key::F2};
};
