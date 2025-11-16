#pragma once

#include "engine/engine.h"

class Application {
public:
    void execute();

private:
    engine::Engine engine_;
};
