#include "ignored_futures.hpp"

#include <spdlog/spdlog.h>

namespace g {

std::unique_ptr<g::IgnoredFutures> ignoredFutures;

}

g::IgnoredFutures::~IgnoredFutures() {
    spdlog::debug("Waiting for all ignored futures to finish...");
    std::lock_guard<std::mutex> lock(mutex);
    for (const std::shared_future<void>& future : futures) {
        future.wait_for(std::chrono::seconds(10));
    }
}
