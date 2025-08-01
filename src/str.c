#include "str.h"
#include <string.h>

int fal_split_displayed_text(const char *text, char delim,
                             char output_buf[][FAL_CONSOLE_WIDTH]) {

  char text_to_split[FAL_CONSOLE_HEIGHT * FAL_CONSOLE_WIDTH];
  strcpy(text_to_split, text);

  int line_count = 0;
  char *split = strtok(text_to_split, "\\");
  while (split) {
    strcpy(output_buf[line_count++], split);
    split = strtok(NULL, "\\");
  }

  return line_count;
}
