#pragma once

#include <memory>

#include "state.h"
#include "step.h"

namespace engine {

class StartupStep : public virtual engine::Step {
public:
    virtual void onStartup(
        const std::shared_ptr<engine::State>& engineState) = 0;
};

}
