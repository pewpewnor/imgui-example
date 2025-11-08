# Xmake Project Template

## 1. Development setup

- Install project dependencies

```bash
# install required packages
xmake require -f
```

- Setup your IDE for the project

```bash
# generate compile commands for clangd (optional)
xmake project -k compile_commands
```

## 2. Run the project

- Build and run

```bash
# set build mode for debug
xmake f -m debug
# set your toolchain if needed
xmake f --toolchain=clang
# build the project
xmake
# run the project
xmake r
```

- Watch for building and running

```bash
xmake watch -c 'xmake f -m debug; xmake; xmake r'
```
