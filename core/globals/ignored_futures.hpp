#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <vector>

namespace g {

class IgnoredFutures {
public:
    std::vector<std::shared_future<void>> futures;
    std::mutex mutex;

    IgnoredFutures() = default;
    IgnoredFutures(const IgnoredFutures&) = delete;
    IgnoredFutures(IgnoredFutures&&) = delete;
    IgnoredFutures& operator=(const IgnoredFutures&) = delete;
    IgnoredFutures& operator=(IgnoredFutures&&) = delete;

    ~IgnoredFutures();
};

extern std::unique_ptr<g::IgnoredFutures> ignoredFutures;

}
