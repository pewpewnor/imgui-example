#ifndef LUA_RENDERER_H
#define LUA_RENDERER_H

#include <string>

class LuaRenderer {
public:
    // Constructor takes the path to a .lua file
    LuaRenderer(const std::string& lua_file);

    // Destructor cleans up Lua state
    ~LuaRenderer();

    // Render method - reads and executes the Lua file
    // Call this every frame to render UI
    bool render();

    // Optional: Change the Lua file at runtime
    void set_lua_file(const std::string& lua_file);

private:
    std::string lua_file_path;
    lua_State* L;
};

#endif  // LUA_RENDERER_H
