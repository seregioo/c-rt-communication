# C RealTime Communication

Realtime communication test on `C` using a comedi. The program takes 10 minutes to run.

## Build

```bash
cmake -B build
cmake --build build
```

With compile commands:

```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build
cmake --build build/
cp build/compile_commands.json .
```

## Execute

```bash
sudo setcap cap_sys_nice=ep build/c_rt_comunication
```
