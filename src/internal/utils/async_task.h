#pragma once

#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "assertion.h"
#include "result.h"

template <typename TResult>
class AsyncTask {
public:
    std::shared_future<void> future;

    AsyncTask(bool invalidateOldCache = false)
        : core_(std::make_shared<TaskCore>()), invalidateOldCache_(invalidateOldCache) {}

    AsyncTask(AsyncTask&&) = delete;
    AsyncTask& operator=(AsyncTask&&) = delete;
    AsyncTask(const AsyncTask&) = delete;
    AsyncTask& operator=(const AsyncTask&) = delete;
    virtual ~AsyncTask() = default;

    [[nodiscard]] bool isAvailable() const { return !isBusy(); }

    [[nodiscard]] bool isBusy() const {
        return future.valid() &&
               future.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
    }

    [[nodiscard]] bool hasResult() const {
        std::lock_guard<std::mutex> lock(core_->outcomeMutex);
        return core_->result.has_value();
    }

    Result<TResult> getResult() {
        std::lock_guard<std::mutex> lock(core_->outcomeMutex);
        if (core_->error.has_value()) {
            std::string errorMessage = std::move(core_->error.value());
            core_->error.reset();
            return std::unexpected{errorMessage};
        }
        ASSERT_HARD(core_->result.has_value(), "result must be available to retrieve");
        return core_->result.value();
    }

protected:
    void spawn(std::function<TResult()> task) {
        ASSERT_HARD(isAvailable(), "must be available to spawn a new task");
        if (invalidateOldCache_) {
            core_ = std::make_shared<TaskCore>();
        } else {
            std::lock_guard<std::mutex> lock(core_->outcomeMutex);
            core_->result.reset();
            core_->error.reset();
        }
        std::shared_ptr<TaskCore> threadCore = core_;
        future = std::async(std::launch::async, [threadCore, task = std::move(task)]() {
                     try {
                         TResult res = task();
                         std::lock_guard<std::mutex> lock(threadCore->outcomeMutex);
                         threadCore->result = std::move(res);
                     } catch (const std::exception& e) {
                         std::lock_guard<std::mutex> lock(threadCore->outcomeMutex);
                         threadCore->error = e.what();
                     } catch (...) {
                         std::lock_guard<std::mutex> lock(threadCore->outcomeMutex);
                         threadCore->error = "unknown async task error";
                     }
                 }).share();
    }

    std::string getErrorMessage() {
        std::lock_guard<std::mutex> lock(core_->outcomeMutex);
        return core_->error.value_or("");
    }

private:
    struct TaskCore {
        std::mutex outcomeMutex;
        Fallible error;
        std::optional<TResult> result;
    };

    std::shared_ptr<TaskCore> core_;
    bool invalidateOldCache_;
};
