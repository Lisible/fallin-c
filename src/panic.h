#ifndef FAL_PANIC_H
#define FAL_PANIC_H

#include <stdio.h>  // IWYU pragma: keep
#include <stdlib.h> // IWYU pragma: keep

#ifdef FAL_WASM
#define FAL_PANIC(...)
#else
#define FAL_PANIC(...)                                                         \
  do {                                                                         \
    printf("PANIC: " __VA_ARGS__);                                             \
    printf("\n");                                                              \
    abort();                                                                   \
  } while (0)
#endif // FAL_WASM

#endif // FAL_PANIC_H
