# C RT Communication

RT communication test on `C` using a comedi. The program takes 10 minutes to run.

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

### Thread backend

For executing the logic, you will need a backend. Default is Posix (non rt).

#### Preempt-RT

```bash
cmake -S . -B build -DRT_BACKEND=PREEMPT_RT
```

After build you will need to set syscap with sudo for being able of executing it with a normal user:

```bash
sudo setcap cap_sys_nice=ep build/c_rt_comunication
```

#### Xenomai

```bash
cmake -S . -B build -DRT_BACKEND=XENOMAI
```

## Execute

```bash
sudo setcap cap_sys_nice=ep build/c_rt_comunication
```
