#pragma once

#include <functional>

class ScopeExit final {
public:
    ScopeExit(std::function<void()> fn);

    ScopeExit(const ScopeExit&) = delete;
    ScopeExit(ScopeExit&&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;
    ScopeExit& operator=(ScopeExit&&) = delete;

    ~ScopeExit();

private:
    std::function<void()> fn_;
};
