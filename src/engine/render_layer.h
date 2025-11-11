#pragma once

#include "layer.h"
#include "rigging.h"

namespace engine {

class RenderLayer : public virtual engine::Layer {
public:
    virtual void render(const std::shared_ptr<engine::Rigging>& rigging) = 0;
};

}
