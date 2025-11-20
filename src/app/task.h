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

    void onTaskSuccess() override { sendRefreshSignal(); }

    void onTaskFailure() override {
        spdlog::error("<{}> Error detected: {}", taskName, this->getErrorMessage());
    }

protected:
    std::string taskName;

    void sendRefreshSignal() {
        spdlog::debug("<{}> Sending refresh signal...", taskName);
        globals::engine->sendRefreshSignal();
    }
};
