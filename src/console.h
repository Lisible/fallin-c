#ifndef FAL_CONSOLE_H
#define FAL_CONSOLE_H

#include <stdbool.h>
#include <stdint.h>

typedef char fal_color;
enum {
  FAL_COLOR_BLACK = 30,
  FAL_COLOR_RED = 31,
  FAL_COLOR_GREEN = 32,
  FAL_COLOR_YELLOW = 33,
  FAL_COLOR_BLUE = 34,
  FAL_COLOR_MAGENTA = 35,
  FAL_COLOR_CYAN = 36,
  FAL_COLOR_WHITE = 37,
  FAL_COLOR_BRIGHTBLACK = 90,
  FAL_COLOR_BRIGHTRED = 91,
  FAL_COLOR_BRIGHTGREEN = 92,
  FAL_COLOR_BRIGHTYELLOW = 93,
  FAL_COLOR_BRIGHTBLUE = 94,
  FAL_COLOR_BRIGHTMAGENTA = 95,
  FAL_COLOR_BRIGHTCYAN = 96,
  FAL_COLOR_BRIGHTWHITE = 97,
};

struct fal_clear_desc {
  fal_color color;
  bool update_background;
};

struct fal_position {
  int x;
  int y;
};

#define FAL_CONSOLE_WIDTH 80
#define FAL_CONSOLE_HEIGHT 25

struct fal_console_char {
  unsigned char character;
  fal_color bg;
  fal_color fg;
};

struct fal_console {
  struct fal_console_char content[FAL_CONSOLE_HEIGHT][FAL_CONSOLE_WIDTH];
  struct fal_position cursor_pos;
  fal_color current_bg_col;
  fal_color current_fg_col;
};
void fal_console_init(struct fal_console *console);
void fal_console_deinit(struct fal_console *console);
void fal_console_clear(struct fal_console *console,
                       const struct fal_clear_desc *desc);
struct fal_position fal_console_position(const struct fal_console *console);
void fal_console_move_to(struct fal_console *console, struct fal_position pos);
void fal_console_move_x(struct fal_console *console, int xrel);
void fal_console_move_y(struct fal_console *console, int yrel);
void fal_console_write_at(struct fal_console *console, struct fal_position pos,
                          const char *text);
void fal_console_draw_rect(struct fal_console *console, struct fal_position tl,
                           struct fal_position br);

void fal_console_render_init(void);
void fal_console_render_deinit(void);
void fal_console_render(
    struct fal_console_char buf[FAL_CONSOLE_HEIGHT][FAL_CONSOLE_WIDTH]);

#endif // FAL_CONSOLE_H
