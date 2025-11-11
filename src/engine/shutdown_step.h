#pragma once

#include "state.h"
#include "step.h"

namespace engine {

class ShutdownStep : public virtual engine::Step {
public:
    virtual void onShutdown(
        const std::shared_ptr<engine::State>& engineState) = 0;
};

}
