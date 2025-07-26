# fallinc

A C re-implementation of a first year students text RPG game engine. Originally
made by [@sliya](https://github.com/sliya), [@metzg](https://github.com/metzg)
and myself.

The goal is to make a C implementation of the Fallin game engine, that can be
compiled into wasm and run on the web.

## Building

### Desktop

You will need ``cmake`` and a C compiler.

```sh
$ cmake -S . -B build
$ cmake --build build # or: cd build/ && make
```

The generated binary will be in build/.

### WASM

You will need ``cmake``, ``clang``, ``libclang-rt-wasm32`` and ``wasi-libc``.

```sh
$ cmake -DCMAKE_TOOLCHAIN_FILE=./cmake/wasm-toolchain.cmake -S . -B build-wasm
$ cmake --build build-wasm # or: cd build-wasm/ && make
```

The generated binary will be in build-wasm/ accompanied by an ``index.html`` file and a ``fallinc.js`` file.
