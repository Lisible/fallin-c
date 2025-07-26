#ifndef FAL_CONSOLE_H
#define FAL_CONSOLE_H

enum fal_color {
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

typedef struct fal_console fal_console;
fal_console *fal_console_create(void);
void fal_console_destroy(fal_console *console);

void fal_console_set_text_color(fal_console *console, enum fal_color color);
void fal_console_set_background_color(fal_console *console,
                                      enum fal_color color);

void fal_console_move(fal_console *console, int xrel, int yrel);
void fal_console_move_to(fal_console *console, int x, int y);

enum fal_frame_border_type {
  FAL_FRAME_BORDER_SINGLE,
  FAL_FRAME_BORDER_DOUBLE,
};
void fal_console_clear(fal_console *console);
void fal_console_draw_frame(fal_console *console, int x1, int y1, int x2,
                            int y2, enum fal_frame_border_type border_type,
                            enum fal_color background_color,
                            enum fal_color text_color);

void fal_console_write_at(fal_console *console, int x, int y, const char *text);

#endif // FAL_CONSOLE_H
