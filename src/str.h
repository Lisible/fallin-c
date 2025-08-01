#ifndef FAL_STR_H
#define FAL_STR_H

#include "console.h"

int fal_split_displayed_text(const char *text, char delim,
                             char output[][FAL_CONSOLE_WIDTH]);

#endif // FAL_STR_H
