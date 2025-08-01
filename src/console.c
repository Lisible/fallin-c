#include "console.h"
#include "platform.h"
#include <string.h>

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

void fal_console_ui_display_illustration(
    fal_console *console, const char illustration[][FAL_CONSOLE_WIDTH],
    int height, int offset) {
  for (int i = 0; i < height; i++) {
    int y = offset + i;
    for (int j = 0; j < FAL_CONSOLE_WIDTH; j++) {
      int x = j;
      fal_console_set_background_color(console,
                                       fal_color_from_char(illustration[i][j]));
      fal_console_write_at(console, x, y, " ");
    }
  }
}

#define FAL_DISPLOC_HEADER_TOP 0
#define FAL_DISPLOC_HEADER_LEFT 0
#define FAL_DISPLOC_HEADER_RIGHT FAL_CONSOLE_WIDTH - 1
#define FAL_DISPLOC_HEADER_BOTTOM 2
#define FAL_DISPLOC_ILLUS_TOP (FAL_DISPLOC_HEADER_BOTTOM + 1)
#define FAL_DISPLOC_ILLUS_BOTTOM (FAL_DISPLOC_ILLUS_TOP + 9)
#define FAL_DISP_PANE_LEFT_TOP FAL_CONSOLE_TOP
#define FAL_DISP_PANE_LEFT_LEFT FAL_CONSOLE_LEFT
#define FAL_DISP_PANE_LEFT_RIGHT (FAL_CONSOLE_WIDTH / 2 - 1)
#define FAL_DISP_PANE_LEFT_BOTTOM (FAL_CONSOLE_HEIGHT - 2)
#define FAL_DISP_PANE_RIGHT_TOP FAL_CONSOLE_TOP
#define FAL_DISP_PANE_RIGHT_LEFT (FAL_CONSOLE_WIDTH / 2)
#define FAL_DISP_PANE_RIGHT_RIGHT FAL_CONSOLE_WIDTH
#define FAL_DISP_PANE_RIGHT_BOTTOM (FAL_CONSOLE_HEIGHT - 2)
#define FAL_DISP_DESCRIPTION_BOX_TOP (FAL_DISPLOC_ILLUS_BOTTOM + 1)
#define FAL_DISP_DESCRIPTION_BOX_LEFT FAL_CONSOLE_LEFT
#define FAL_DISP_DESCRIPTION_BOX_RIGHT FAL_CONSOLE_WIDTH - 1
#define FAL_DISP_DESCRIPTION_BOX_BOTTOM (FAL_CONSOLE_HEIGHT - 2)
#define FAL_DISPLOC_DESCRIPTION_TOP (FAL_DISPLOC_ILLUS_BOTTOM + 1)
#define FAL_DISPLOC_DESCRIPTION_LEFT FAL_CONSOLE_LEFT
#define FAL_DISPLOC_DESCRIPTION_RIGHT FAL_CONSOLE_WIDTH - 1
#define FAL_DISPLOC_DESCRIPTION_BOTTOM (FAL_CONSOLE_HEIGHT - 2)

void fal_console_ui_display_interface(fal_console *console,
                                      enum fal_ui_position position,
                                      enum fal_border_type border,
                                      const char content[][FAL_CONSOLE_WIDTH],
                                      int content_size, bool input_line) {
  enum fal_color background_color = FAL_COLOR_WHITE;
  enum fal_color text_color = FAL_COLOR_BLACK;

  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;

  switch (position) {
  case FAL_UI_POSITION_HEADER:
    x1 = FAL_DISPLOC_HEADER_LEFT;
    y1 = FAL_DISPLOC_HEADER_TOP;
    x2 = FAL_DISPLOC_HEADER_RIGHT;
    y2 = FAL_DISPLOC_HEADER_BOTTOM;
    break;
  case FAL_UI_POSITION_LEFT_PANE:
    x1 = FAL_DISP_PANE_LEFT_LEFT;
    y1 = FAL_DISP_PANE_LEFT_TOP;
    x2 = FAL_DISP_PANE_LEFT_RIGHT;
    y2 = FAL_DISP_PANE_LEFT_BOTTOM;
    break;
  case FAL_UI_POSITION_RIGHT_PANE:
    x1 = FAL_DISP_PANE_RIGHT_LEFT;
    y1 = FAL_DISP_PANE_RIGHT_TOP;
    x2 = FAL_DISP_PANE_RIGHT_RIGHT;
    y2 = FAL_DISP_PANE_RIGHT_BOTTOM;
    break;
  case FAL_UI_POSITION_DESCRIPTION_BOX:
    x1 = FAL_DISP_DESCRIPTION_BOX_LEFT;
    y1 = FAL_DISP_DESCRIPTION_BOX_TOP;
    x2 = FAL_DISP_DESCRIPTION_BOX_RIGHT;
    y2 = FAL_DISP_DESCRIPTION_BOX_BOTTOM;
    break;
  case FAL_UI_POSITION_POP_BOX:
    x1 = FAL_CONSOLE_WIDTH / 4;
    y1 = (FAL_CONSOLE_HEIGHT / 2) - ((content_size + 2) / 2);
    x2 = 3 * FAL_CONSOLE_WIDTH / 4;
    y2 = (FAL_CONSOLE_HEIGHT / 2) + ((content_size + 2) / 2);
    background_color = FAL_COLOR_YELLOW;
    break;
  case FAL_UI_POSITION_BOTTOM:
    x1 = FAL_DISP_DESCRIPTION_BOX_LEFT;
    y1 = FAL_CONSOLE_HEIGHT - content_size - 2;
    x2 = FAL_DISP_DESCRIPTION_BOX_RIGHT;
    y2 = FAL_CONSOLE_HEIGHT - 1;
    break;
  }

  fal_console_draw_frame(console, x1, y1, x2, y2, border, background_color,
                         text_color);
  int write_x = x1 + 1;
  int write_y = y1 + 1;
  for (int i = 0; i < content_size; i++) {
    fal_console_write_at(console, write_x, write_y, content[i]);
    write_y++;
  }

  if (input_line) {
    fal_console_move_to(console, write_x, write_y);
  }
}

void fal_console_ui_display_notice_pop(fal_console *console,
                                       const char *content) {
  char c[FAL_CONSOLE_WIDTH * FAL_CONSOLE_HEIGHT] = {0};
  strcpy(c, content);

  int line_count = 0;
  char notice_pop_content[FAL_CONSOLE_HEIGHT][FAL_CONSOLE_WIDTH] = {0};
  char *split = strtok(c, "\\");
  while (split) {
    strcpy(notice_pop_content[line_count++], split);
    split = strtok(NULL, "\\");
  }

  fal_console_ui_display_interface(console, FAL_UI_POSITION_POP_BOX,
                                   FAL_BORDER_SINGLE, notice_pop_content,
                                   line_count, false);
  fal_platform_read_key();
}
