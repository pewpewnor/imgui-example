#include "debug_key_handler.hpp"

#include "globals/dispatch_state.hpp"

void DebugKeyHandler::onRender() {
    g::dispatch->showImguiDemoWindow = KeyPressDetector::combineKeyPressAndKeyHeld(
        f1Key_, f2Key_, g::dispatch->showImguiDemoWindow);
}
