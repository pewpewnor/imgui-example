#pragma once

#include "spdlog/spdlog.h"
#include "utils/async_result.h"

class SleepTask : public AsyncResult<std::string> {
public:
    void execute(const std::string& name, int frameCount) {
        spdlog::debug("Spawned sleep worker (Class based)");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        spdlog::debug("Finished sleeping");
        std::string res = "Hello, " + name + " " + std::to_string(frameCount) + "!";
        this->submitResult(res);
    }
};
