#pragma once

#include <exception>
#include <expected>
#include <future>
#include <mutex>
#include <optional>

#include "assertion.h"
#include "result.h"

template <typename TResult>
class AsyncTask {
public:
    AsyncTask(bool invalidateOldCache = false) : invalidateOldCache_(invalidateOldCache) {}

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
        std::lock_guard<std::mutex> lock(outcomeMutex_);
        return result_.has_value();
    }

    Result<TResult> getResult() {
        std::lock_guard<std::mutex> lock(outcomeMutex_);
        if (error_.has_value()) {
            std::string errorMessage = std::move(error_.value());
            error_.reset();
            return std::unexpected{errorMessage};
        }
        ASSERT_HARD(result_.has_value(), "result must be available to retrieve");
        return result_.value();
    }

protected:
    std::shared_future<void> future;

    virtual void task() = 0;

    virtual void onTaskSuccess() {}

    virtual void onTaskFailure() {}

    void spawn() {
        ASSERT_HARD(isAvailable(), "must not be busy to run new task");
        if (invalidateOldCache_) {
            std::lock_guard<std::mutex> lock(outcomeMutex_);
            result_.reset();
        }
        error_.reset();
        spawnFuture();
    }

    void submitResult(const TResult& value) {
        std::lock_guard<std::mutex> lock(outcomeMutex_);
        result_ = std::move(value);
    }

    std::string getErrorMessage() {
        ASSERT_HARD(error_.has_value(), "an error must have occured");
        return error_.value();
    }

private:
    bool invalidateOldCache_;
    mutable std::mutex outcomeMutex_;
    Fallible error_;
    std::optional<TResult> result_;

    void spawnFuture() {
        future = std::async(std::launch::async, [this]() {
                     try {
                         this->task();
                         this->onTaskSuccess();
                     } catch (const std::exception& error) {
                         {
                             std::lock_guard<std::mutex> lock(outcomeMutex_);
                             this->error_ = error.what();
                         }
                         try {
                             this->onTaskFailure();
                         } catch (...) {
                             ;
                         }
                     } catch (...) {
                         {
                             std::lock_guard<std::mutex> lock(outcomeMutex_);
                             this->error_ = "unknown async task error";
                         }
                         try {
                             this->onTaskFailure();
                         } catch (...) {
                             ;
                         }
                     }
                 }).share();
    }
};
