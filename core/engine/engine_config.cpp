#include "engine_config.hpp"

engine::EngineConfig::EngineConfig(bool insistRenderMode, int activeFps, int idleFps)
    : insistRenderMode(insistRenderMode), activeFps(activeFps), idleFps(idleFps) {}
