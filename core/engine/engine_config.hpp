#pragma once

namespace engine {

class EngineConfig {
public:
    bool insistRenderMode;
    int activeFps;
    int idleFps;

    EngineConfig(bool insistRenderMode, int activeFps, int idleFps);
};

}
