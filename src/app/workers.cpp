#include "workers.h"

#include "spdlog/spdlog.h"
#include "utils/result.h"

Workers::~Workers() {
    spdlog::info("Finishing remaining async workers...");
    if (Fallible error = sleepWorker.kill()) {
        spdlog::error("Failed to kill sleep worker: {}", error.value());
    }
}
