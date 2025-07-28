#include "jbb.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

bool fal_jbb_read_line(FILE *file, char *buf, size_t buf_cap) {
  char *ret = fgets(buf, buf_cap, file);
  if (!ret) {
    return false;
  }

  int line_len = strlen(buf);
  if (line_len > 0 && buf[line_len - 1] == '\n') {
    buf[line_len - 1] = '\0';
  }

  return true;
}

bool fal_jbb_parse_int(FILE *file, int *value) {
  char line_buf[512];
  fal_jbb_read_line(file, line_buf, 512);
  *value = (int)strtol(line_buf, NULL, 10);
  return true;
}
bool fal_jbb_parse_string(FILE *file, char *value, size_t capacity) {
  char line_buf[512];
  fal_jbb_read_line(file, line_buf, 512);
  memcpy(value, line_buf, capacity);
  return true;
}

static void to_uppercase(char *str) {
  while (*str) {
    *str = toupper((unsigned char)*str);
    str++;
  }
}

bool fal_jbb_parse_bool(FILE *f, bool *value) {
  char line_buf[512];
  if (!fal_jbb_read_line(f, line_buf, 512)) {
    return false;
  }

  to_uppercase(line_buf);
  *value = strcmp(line_buf, "TRUE") == 0;
  return true;
}
