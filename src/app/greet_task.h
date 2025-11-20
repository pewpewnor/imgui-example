#pragma once

#include <cstring>

#include "spdlog/spdlog.h"
#include "task.h"

class GreetTask : public Task<std::string> {
public:
    GreetTask() : Task("GreetTask") {}

    void execute(const std::string& name, int frame) {
        name_ = name;
        frame_ = frame;
        this->spawn();
    }

private:
    std::string name_;
    int frame_ = 0;

    void task() override {
        spdlog::debug("<Greet Task> Starting sleep...");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        /* if (frame_ > 300) {
            throw std::runtime_error("this is a test runtime error");
        } */
        std::string res = "Hello, " + name_ + "! (Frame: " + std::to_string(frame_) + ")";
        this->submitResult(res);
    }
};
