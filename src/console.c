#include "console.h"
#include <stddef.h>
#include <string.h>

void fal_console_init(struct fal_console *console) {
  console->current_bg_col = FAL_COLOR_BLACK;
  console->current_text_color = FAL_COLOR_WHITE;
  console->cursor_pos.x = 0;
  console->cursor_pos.y = 0;
  fal_console_clear(console, NULL);
  fal_console_render_init();
}
void fal_console_deinit(struct fal_console *console) {
  (void)console;
  fal_console_render_deinit();
}
void fal_console_clear(struct fal_console *console,
                       const struct fal_clear_desc *desc) {
  if (desc && desc->update_background) {
    console->current_bg_col = desc->color;
  }

  for (int y = 0; y < FAL_CONSOLE_HEIGHT; y++) {
    for (int x = 0; x < FAL_CONSOLE_WIDTH; x++) {
      console->content[y][x].bg = console->current_bg_col;
      console->content[y][x].fg = console->current_text_color;
      console->content[y][x].character = ' ';
    }
  }
}
struct fal_position fal_console_position(const struct fal_console *console) {
  return console->cursor_pos;
}
void fal_console_move_to(struct fal_console *console, struct fal_position pos) {
  console->cursor_pos = pos;
}
void fal_console_move_x(struct fal_console *console, int xrel) {
  console->cursor_pos.x += xrel;
}
void fal_console_move_y(struct fal_console *console, int yrel) {
  console->cursor_pos.y += yrel;
}
void fal_console_write_at(struct fal_console *console, struct fal_position pos,
                          const char *text) {
  fal_console_move_to(console, pos);
  size_t text_length = strlen(text);
  for (int i = 0; i < text_length; i++) {
    struct fal_console_char *console_char =
        &console->content[console->cursor_pos.y][console->cursor_pos.x];
    console_char->character = text[i];
    console_char->bg = console->current_bg_col;
    console_char->fg = console->current_text_color;

    console->cursor_pos.x++;
    if (console->cursor_pos.x >= FAL_CONSOLE_WIDTH) {
      console->cursor_pos.x = 0;
      console->cursor_pos.y++;
    }
  }
}
void fal_console_set_text_color(struct fal_console *console, fal_color text) {
  console->current_text_color = text;
}
void fal_console_set_bg_color(struct fal_console *console, fal_color bg) {

  console->current_bg_col = bg;
}
void fal_console_set_colors(struct fal_console *console, fal_color bg,
                            fal_color text) {
  fal_console_set_bg_color(console, bg);
  fal_console_set_text_color(console, text);
}
void fal_console_draw_frame(struct fal_console *console,
                            const struct fal_console_frame_infos *frame_infos) {
  struct fal_position tl = frame_infos->top_left;
  struct fal_position br = frame_infos->bottom_right;
  fal_console_set_colors(console, frame_infos->background_color,
                         frame_infos->text_color);

  if (br.x < tl.x || br.y < tl.y) {
    return;
  }

  struct fal_console_char(*content)[FAL_CONSOLE_HEIGHT][FAL_CONSOLE_WIDTH] =
      &console->content;

  for (int y = tl.y; y <= br.y; y++) {
    for (int x = tl.x; x <= br.x; x++) {
      (*content)[y][x].bg = console->current_bg_col;
      (*content)[y][x].fg = console->current_text_color;

      char c;
      if ((x == tl.x && y == tl.y) || (x == br.x && y == tl.y) ||
          (x == tl.x && y == br.y) || (x == br.x && y == br.y)) {
        c = frame_infos->border_type == FAL_FRAME_BORDER_SINGLE ? '+' : '#';
        (*content)[y][x].character = c;
      } else if (x == tl.x || x == br.x) {
        c = frame_infos->border_type == FAL_FRAME_BORDER_SINGLE ? '|' : '#';
        (*content)[y][x].character = c;
      } else if (y == tl.y || y == br.y) {
        c = frame_infos->border_type == FAL_FRAME_BORDER_SINGLE ? '-' : '=';
        (*content)[y][x].character = c;
      } else {
        (*content)[y][x].character = ' ';
      }
    }
  }
}
