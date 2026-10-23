# ImGui and SFML Application Example

A C++ desktop application prototype built around Dear ImGui and SFML. It
demonstrates an application lifecycle and a small custom engine with render
layers, asynchronous task handling, and a couple of sample windows. It is useful
as a starting point for experimenting with an immediate-mode GUI app.

## Stack

C++23, Xmake, SFML, Dear ImGui through ImGui-SFML, and spdlog. The repository
also contains the ImGui-SFML dependency as a Git submodule.

## Build and run

Install Xmake and a Clang toolchain. Clone with submodules so the vendored
dependency is available:

~~~sh
git clone --recurse-submodules https://github.com/pewpewnor/imgui-example.git
cd imgui-example
xmake f -m debug
xmake
xmake r app
~~~

Xmake resolves the declared packages during configuration. Debug builds enable
address, undefined-behavior, and leak sanitizers where supported. Use `xmake f
-m release` for a release build.
