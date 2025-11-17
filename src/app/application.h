#pragma once

#include "app/globals.h"
#include "engine/engine.h"

class Application {
public:
    void execute();

private:
    std::unique_ptr<engine::Engine> engine_ =
        std::make_unique<engine::Engine>(globals::engineState);
};
