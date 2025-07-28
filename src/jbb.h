#ifndef FAL_JBB_H
#define FAL_JBB_H

#include <stdbool.h>
#include <stdio.h>

bool fal_jbb_read_line(FILE *file, char *buf, size_t buf_cap);
bool fal_jbb_parse_int(FILE *f, int *value);
bool fal_jbb_parse_string(FILE *f, char *value, size_t capacity);
bool fal_jbb_parse_bool(FILE *f, bool *value);

#endif // FAL_JBB_H
