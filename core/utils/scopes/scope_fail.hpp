#pragma once

#include <functional>
#include <string>

class ScopeFail final {
public:
    ScopeFail(std::function<void(const std::string&)> fn);

    ScopeFail(const ScopeFail&) = delete;
    ScopeFail(ScopeFail&&) = delete;
    ScopeFail& operator=(const ScopeFail&) = delete;
    ScopeFail& operator=(ScopeFail&&) = delete;

    ~ScopeFail();

private:
    std::function<void(const std::string&)> fn_;
};
