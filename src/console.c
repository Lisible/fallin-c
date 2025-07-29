#include "console.h"
#include "panic.h"

static char color_char_to_byte(char color) {
  if (color >= '0' && color <= '9') {
    return color - '0' + 30;
  }
  switch (color) {
  case 'A':
    return 10 + 80;
  case 'B':
    return 11 + 80;
  case 'C':
    return 12 + 80;
  case 'D':
    return 13 + 80;
  case 'E':
    return 14 + 80;
  case 'F':
    return 15 + 80;
  }

  FAL_PANIC("Invalid color");
  return 0;
}

void fal_console_ui_display_illustration(fal_console *console,
                                         const char illustration[][80],
                                         int height, int offset) {
  for (int i = 0; i < height; i++) {
    int y = offset + i;
    for (int j = 0; j < 80; j++) {
      int x = j;
      fal_console_set_background_color(console,
                                       color_char_to_byte(illustration[i][j]));
      fal_console_set_text_color(console,
                                 color_char_to_byte(illustration[i][j]));
      fal_console_write_at(console, x, y, &illustration[i][j]);
    }
  }
}
