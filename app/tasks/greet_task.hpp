#pragma once

#include "task.hpp"

class GreetTask : public Task<std::string> {
public:
    void execute(const std::string& name, int frame);

protected:
    [[nodiscard]] std::string getTaskId() override;
};
