#pragma once

#include "engine_state.h"
#include "ignored_tasks.h"
#include "spdlog/spdlog.h"
#include "utils/async_task.h"

template <typename TResult>
class Task : public virtual AsyncTask<TResult> {
public:
    Task(std::string_view taskName) : taskName(taskName) {}

    Task(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;
    virtual ~Task() = default;

    void ignore() {
        if (this->isBusy()) {
            spdlog::debug("ignoring task...");
            std::lock_guard<std::mutex> lock(ignored_tasks::ignoredFuturesMutex);
            ignored_tasks::ignoredFutures.push_back(std::move(this->future));
        }
    }

protected:
    std::string taskName;

    void spawnTask(std::function<TResult()> task) {
        std::string nameCapture = taskName;
        this->spawn([nameCapture, task = std::move(task)]() -> TResult {
            try {
                TResult result = task();
                if (globals::engine) {
                    spdlog::debug("<{}> Sending refresh signal...", nameCapture);
                    globals::engine->sendRefreshSignal();
                }
                return result;
            } catch (const std::exception& e) {
                spdlog::error("<{}> Error detected: {}", nameCapture, e.what());
                throw;
            }
        });
    }
};
