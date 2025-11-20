#include <future>
#include <mutex>

#include "spdlog/spdlog.h"

namespace ignored_tasks {

inline std::vector<std::shared_future<void>> ignoredFutures;
inline std::mutex ignoredFuturesMutex;

inline void waitAllIgnoredFutures(std::chrono::seconds waitTimeSeconds) {
    spdlog::debug("Waiting for all ignored futures to finish...");
    std::lock_guard<std::mutex> lock(ignoredFuturesMutex);
    for (const std::shared_future<void>& future : ignoredFutures) {
        future.wait_for(waitTimeSeconds);
    }
}

}
