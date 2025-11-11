#pragma once

namespace engine {

class Layer {
public:
    Layer() = default;

    Layer(const Layer&) = default;
    Layer(Layer&&) = delete;
    Layer& operator=(const Layer&) = default;
    Layer& operator=(Layer&&) = delete;
    virtual ~Layer() = default;
};

}
