# TPE Arquitectura de las Computadoras (72.02)

## Members

| Name                   | Student ID | Email                      |
|------------------------|------------|----------------------------|
| Lucia Oliveto          | 64646      | loliveto@itba.edu.ar       |
| Máximo Wehncke         | 64018      | mwehncke@itba.edu.ar       |
| Tomas Pietravallo      | 64288      | tpietravallo@itba.edu.ar   |

# Compilation
To compile the project simply run the `./compile.sh` files (use `./run` to launch). Make sure to have [Docker](http://docker.com/products/docker-desktop/) installed and running on your system

```sh
./compile.sh && ./run.sh
```

## Syscalls

System calls which replicate standard linux/posix use [the same IDs and arguments as their original counterparts](https://syscalls.gael.in)

System calls particular to this particular project have IDs in the `0x80000000` range. To call this interruptions simply use the appropiate code and arguments

```nasm
mov eax, 0x80000000 ; sys_beep
int 80h ; triggers a :beep: sound
```

## Userspace

There are two libraries provided alongside the userspace code modules:

- `libc.a` A statically compiled library archive containing some of C's standard library functions
- `sys.a` A static library archive containing functions specific to this project (like `beep`)
