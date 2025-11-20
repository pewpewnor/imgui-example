#pragma once

#include <mutex>
#include <optional>

#include "utils/assertion.h"

template <typename TResult>
class AsyncResult {
public:
    [[nodiscard]] bool hasResult() const {
        std::lock_guard<std::mutex> lock(mutex);
        return result.has_value();
    }

    TResult getResult() {
        std::lock_guard<std::mutex> lock(mutex);
        ASSERT_HARD(result.has_value(), "result must be available");
        auto val = std::move(result.value());
        result.reset();
        return val;
    }

    void invalidate() {
        std::lock_guard<std::mutex> lock(mutex);
        result.reset();
    }

protected:
    void submitResult(const TResult& value) {
        std::lock_guard<std::mutex> lock(mutex);
        result = std::move(value);
    }

    mutable std::mutex mutex;
    std::optional<TResult> result;
};
