#pragma once

#include <spdlog/spdlog.h>

#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "globals/engine_state.hpp"
#include "globals/ignored_futures.hpp"
#include "utils/assertions.hpp"
#include "utils/results.hpp"
#include "utils/scopes/scope_fail.hpp"

template <typename TResult>
using TaskFunction = std::function<TResult()>;
template <typename TResult>
using SuccessCallback = std::function<void(const TResult& result)>;
using FailureCallback = std::function<void(const std::string& errorMsg)>;

template <typename TResult>
class AsyncTask {
public:
    AsyncTask(bool invalidateOldCache = false) : invalidateOldCache_(invalidateOldCache) {}

    AsyncTask(AsyncTask&&) = delete;
    AsyncTask& operator=(AsyncTask&&) = delete;
    AsyncTask(const AsyncTask&) = delete;
    AsyncTask& operator=(const AsyncTask&) = delete;

    virtual ~AsyncTask() { ignore(); }

    [[nodiscard]] bool isAvailable() { return !isBusy(); }

    [[nodiscard]] bool isBusy() {
        return future_.valid() &&
               future_.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
    }

    [[nodiscard]] bool hasResult() {
        std::lock_guard<std::mutex> lock(outcome_->mutex);
        return outcome_->result.has_value();
    }

    Result<TResult> getResult() {
        std::lock_guard<std::mutex> lock(outcome_->mutex);
        if (outcome_->error.has_value()) {
            std::string errorMessage = outcome_->error.value();
            outcome_->error.reset();
            return std::unexpected{errorMessage};
        }
        ASSERT(outcome_->result.has_value(), "task result or error must be available to retrieve");
        return outcome_->result.value();
    }

    void ignoreCurrentTask() {
        ignore();
        spdlog::debug("<{}> This task has been ignored...", getTaskId());
    }

protected:
    [[nodiscard]] virtual std::string getTaskId() = 0;

    void spawnTask(const TaskFunction<TResult>& task) {
        std::string taskId = getTaskId();
        auto onSuccess = [taskId](const TResult&) {
            spdlog::debug("<{}> Task successfully done", taskId);
            g::engine->sendRefreshSignal();
        };
        auto onFailure = [taskId](std::string_view errorMsg) {
            spdlog::error("<{}> Task error: {}", taskId, errorMsg);
        };
        this->spawnTaskWithCallbacks(task, std::move(onSuccess), std::move(onFailure));
    }

    void spawnTaskWithCallbacks(TaskFunction<TResult> task, SuccessCallback<TResult> onSuccess,
                                FailureCallback onFailure) {
        ASSERT(task, "task function must be defined");
        ASSERT(onSuccess, "onSuccess callback function must be defined");
        ASSERT(onFailure, "onFailure callback function must be defined");
        ASSERT(isAvailable(), "must be available to spawn a new task");

        std::shared_ptr<TaskOutcome> prevCore = outcome_;
        outcome_ = std::make_shared<TaskOutcome>();
        if (!invalidateOldCache_ && prevCore) {
            std::lock_guard<std::mutex> lockPrevCore(prevCore->mutex);
            std::lock_guard<std::mutex> lockCore(outcome_->mutex);
            outcome_->result = prevCore->result;
        }

        future_ = std::async(std::launch::async, [outcome = this->outcome_, task = std::move(task),
                                                  onSuccess = std::move(onSuccess),
                                                  onFailure = std::move(onFailure)]() {
                      try {
                          ScopeFail scopeFail([&](const std::string& errorMsg) {
                              {
                                  std::lock_guard<std::mutex> lock(outcome->mutex);
                                  outcome->error = errorMsg;
                              }
                              onFailure(errorMsg);
                          });
                          TResult res = task();
                          {
                              std::lock_guard<std::mutex> lock(outcome->mutex);
                              outcome->result = res;
                          }
                          onSuccess(res);
                      } catch (...) {
                          ;
                      }
                  }).share();
    }

private:
    struct TaskOutcome {
        std::mutex mutex;
        Fallible error;
        std::optional<TResult> result;
    };

    std::shared_ptr<TaskOutcome> outcome_ = std::make_shared<TaskOutcome>();
    std::shared_future<void> future_;
    bool invalidateOldCache_;

    void ignore() {
        if (this->isBusy()) {
            std::lock_guard<std::mutex> lock(g::ignoredFutures->mutex);
            g::ignoredFutures->futures.push_back(std::move(this->future_));
        }
        ASSERT(this->isAvailable(), "task must be free after ignore");
    }
};
