#pragma once

#include "sleep_task.h"
#include "utils/async_executor.h"

struct Workers {
    AsyncExecutor executor;
    SleepTask sleepTask;
};
