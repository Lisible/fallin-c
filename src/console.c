#include "console.h"
#include "panic.h"

static enum fal_color fal_color_from_char(char color) {
  switch (color) {
  case '0':
    return FAL_COLOR_BLACK;
  case '1':
    return FAL_COLOR_BLUE;
  case '2':
    return FAL_COLOR_GREEN;
  case '3':
    return FAL_COLOR_CYAN;
  case '4':
    return FAL_COLOR_RED;
  case '5':
    return FAL_COLOR_MAGENTA;
  case '6':
    return FAL_COLOR_YELLOW;
  case '7':
    return FAL_COLOR_WHITE;
  case '8':
    return FAL_COLOR_BRIGHTBLACK;
  case '9':
    return FAL_COLOR_BRIGHTBLUE;
  case 'A':
    return FAL_COLOR_BRIGHTGREEN;
  case 'B':
    return FAL_COLOR_BRIGHTCYAN;
  case 'C':
    return FAL_COLOR_BRIGHTRED;
  case 'D':
    return FAL_COLOR_BRIGHTMAGENTA;
  case 'E':
    return FAL_COLOR_BRIGHTYELLOW;
  case 'F':
    return FAL_COLOR_BRIGHTWHITE;
  default:
    return FAL_COLOR_MAGENTA;
  }
}

void fal_console_ui_display_illustration(fal_console *console,
                                         const char illustration[][80],
                                         int height, int offset) {
  for (int i = 0; i < height; i++) {
    int y = offset + i;
    for (int j = 0; j < 80; j++) {
      int x = j;
      fal_console_set_background_color(console,
                                       fal_color_from_char(illustration[i][j]));
      fal_console_write_at(console, x, y, " ");
    }
  }
}
