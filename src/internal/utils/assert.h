#pragma once

#include <spdlog/spdlog.h>

#include <cassert>

#ifndef NDEBUG
#define ASSERT(condition, message) assert((condition) && (message))
#else
#define ASSERT(condition, message)                          \
    do {                                                    \
        if (!(condition)) {                                 \
            spdlog::error("Assertion failed: {}", message); \
        }                                                   \
    } while (0)
#endif
