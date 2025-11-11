#pragma once

#include "layer.h"
#include "rigging.h"

namespace engine {

class ShutdownLayer : public virtual engine::Layer {
public:
    virtual void execute(const std::shared_ptr<engine::Rigging>& rigging) = 0;
};

}
