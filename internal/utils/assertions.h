#pragma once

#ifndef NDEBUG
#include <cassert>

#define ASSERT_DEBUG(condition, message) assert((condition) && (message))
#define ASSERT_HARD(condition, message) ASSERT_DEBUG(condition, message)
#define ASSERT_SOFT(condition, message) ASSERT_DEBUG(condition, message)
#else
#include <spdlog/spdlog.h>

#include <stdexcept>
#include <string>

#define ASSERT_DEBUG(condition, message)

#define ASSERT_HARD(condition, message)                            \
    do {                                                           \
        if (!(condition)) {                                        \
            spdlog::error("Hard assertion failed: {}", (message)); \
            throw std::runtime_error(message);                     \
        }                                                          \
    } while (0)

#define ASSERT_SOFT(condition, message)                            \
    do {                                                           \
        if (!(condition)) {                                        \
            spdlog::error("Soft assertion failed: {}", (message)); \
        }                                                          \
    } while (0)
#endif
