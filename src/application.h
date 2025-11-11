#pragma once

#include "engine/engine.h"
#include "engine/state.h"

class SharedState : public engine::State {
public:
    SharedState() : engine::State() {}
};

class Application {
public:
    void run();

private:
    std::unique_ptr<engine::Engine<SharedState>> engine_;
};
