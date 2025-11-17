#include <chrono>
#include <expected>
#include <future>
#include <string>

template <typename T>
class AsyncWorker {
private:
    std::future<T> future_;

public:
    template <typename TFunc, typename... TArgs>
    void spawn(TFunc&& func, TArgs&&... args) {
        future_ = std::async(std::launch::async, std::forward<TFunc>(func),
                             std::forward<TArgs>(args)...);
    }

    [[nodiscard]] bool resultIsReady() const {
        if (!future_.valid()) {
            return false;
        }
        return future_.wait_for(std::chrono::seconds(0)) ==
               std::future_status::ready;
    }

    [[nodiscard]] bool isBusyWorking() const {
        return future_.valid() && !resultIsReady();
    }

    std::expected<T, std::string> getResultBlocking() {
        if (!future_.valid()) {
            return std::unexpected("No valid future to retrieve.");
        }
        try {
            return future_.get();
        } catch (const std::exception& e) {
            return std::unexpected(e.what());
        } catch (...) {
            return std::unexpected("Unknown asynchronous error occurred.");
        }
    }
};
