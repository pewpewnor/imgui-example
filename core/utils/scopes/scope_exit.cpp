#include "scope_exit.hpp"

ScopeExit::ScopeExit(std::function<void()> fn) : fn_(std::move(fn)) {}

ScopeExit::~ScopeExit() { fn_(); }
