#ifndef FAL_PANIC_H
#define FAL_PANIC_H

#include <stdio.h>  // IWYU pragma: keep
#include <stdlib.h> // IWYU pragma: keep

#define FAL_PANIC(...)                                                         \
  do {                                                                         \
    fprintf(stderr, "PANIC: " __VA_ARGS__);                                    \
    fprintf(stderr, "\n");                                                     \
    abort();                                                                   \
  } while (0)

#endif // FAL_PANIC_H
