#pragma once

#include "utils/async_worker.h"

class Workers {
public:
    AsyncWorker<std::string> sleepWorker;

    Workers() = default;
    Workers(const Workers&) = delete;
    Workers(Workers&&) = delete;
    Workers& operator=(const Workers&) = delete;
    Workers& operator=(Workers&&) = delete;

    ~Workers();
};
