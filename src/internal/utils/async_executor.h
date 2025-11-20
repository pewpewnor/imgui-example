#pragma once

#include <future>

#include "utils/assertion.h"

class AsyncExecutor {
public:
    AsyncExecutor() = default;
    AsyncExecutor(const AsyncExecutor&) = delete;
    AsyncExecutor(AsyncExecutor&&) = delete;
    AsyncExecutor& operator=(const AsyncExecutor&) = delete;
    AsyncExecutor& operator=(AsyncExecutor&&) = delete;

    ~AsyncExecutor() {
        if (future_.valid()) {
            future_.wait();
        }
    }

    [[nodiscard]] bool isBusy() const {
        return future_.valid() &&
               future_.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
    }

    template <typename TaskObj, typename TMethod, typename TCleanUpFunc, typename... TArgs>
    void spawn(TaskObj* taskInstance, TMethod method, TCleanUpFunc postTask, TArgs&&... args) {
        ASSERT_SOFT(!isBusy(), "must not spawn on a busy executor");
        taskInstance->invalidate();
        future_ = std::async(std::launch::async, [taskInstance, method, postTask,
                                                  argsTuple = std::make_tuple(
                                                      std::forward<TArgs>(args)...)]() mutable {
            std::apply([&](auto&&... unpackedArgs) { (taskInstance->*method)(unpackedArgs...); },
                       std::move(argsTuple));
            postTask();
        });
    }

private:
    std::future<void> future_;
};
