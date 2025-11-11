#pragma once

#include "state.h"
#include "step.h"

namespace engine {

class RenderStep : public virtual engine::Step {
public:
    virtual void onRender(
        const std::shared_ptr<engine::State>& engineState) = 0;
};

}
