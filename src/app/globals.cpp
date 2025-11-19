#include "globals.h"

#include "spdlog/spdlog.h"

Workers::~Workers() {
    spdlog::info("Finishing remaining async workers...");
    if (Fallible error = sleepWorker.kill()) {
        spdlog::error("Failed to kill sleep worker: {}", error.value());
    }
}
