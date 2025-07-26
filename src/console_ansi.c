#include "console.h"
#include "panic.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define FAL_CONSOLE_WIDTH 80
#define FAL_CONSOLE_HEIGHT 25

struct fal_console {
  enum fal_color background_color;
  enum fal_color text_color;
};

static struct termios orig_termios;
fal_console *fal_console_create(void) {
  fal_console *console = malloc(sizeof(struct fal_console));
  if (!console) {
    FAL_PANIC("Couldn't allocate memory for console");
  }

  console->background_color = FAL_COLOR_BLACK;
  console->text_color = FAL_COLOR_WHITE;
  if (tcgetattr(STDIN_FILENO, &orig_termios) != 0) {
    FAL_PANIC("Couldn't get terminal parameter informations: %s",
              strerror(errno));
  }
  struct termios raw;
  raw = orig_termios;
  raw.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | INPCK | ISTRIP | IXON);
  raw.c_oflag = 0;
  raw.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  raw.c_cflag &= ~(CSIZE | PARENB);
  raw.c_cflag |= CS8;
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0) {
    FAL_PANIC("Couldn't set terminal parameter informations: %s",
              strerror(errno));
  }

  return console;
}
void fal_console_destroy(fal_console *console) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) != 0) {
    FAL_PANIC("Couldn't set terminal parameter informations: %s",
              strerror(errno));
  }
  printf("\033[0m\n");
  free(console);
}

void fal_console_set_text_color(fal_console *console, enum fal_color color) {
  console->text_color = color;
}
void fal_console_set_background_color(fal_console *console,
                                      enum fal_color color) {
  console->background_color = color;
}

void fal_console_move(fal_console *console, int xrel, int yrel) {
  if (xrel > 0) {
    printf("\033[%dC", xrel);
  } else {
    printf("\033[%dD", -xrel);
  }
  if (yrel > 0) {
    printf("\033[%dB", yrel);
  } else {
    printf("\033[%dA", -yrel);
  }
}
void fal_console_move_to(fal_console *console, int x, int y) {
  printf("\033[%d;%dH", y, x);
}

void fal_console_clear(fal_console *console) {
  printf("\033[%d;%dm", console->background_color + 10, console->text_color);

  for (int y = 0; y < FAL_CONSOLE_HEIGHT; y++) {
    for (int x = 0; x < FAL_CONSOLE_WIDTH; x++) {
      fal_console_move_to(console, x, y);
      printf(" ");
    }
  }
}
void fal_console_draw_frame(fal_console *console, int x1, int y1, int x2,
                            int y2, enum fal_frame_border_type border_type,
                            enum fal_color background_color,
                            enum fal_color text_color) {
  if (x1 > x2 || y1 > y2) {
    return;
  }

  fal_console_set_background_color(console, background_color);
  fal_console_set_text_color(console, text_color);
  printf("\033[%d;%dm", console->background_color + 10, console->text_color);

  for (int y = y1; y <= y2; y++) {
    for (int x = x1; x <= x2; x++) {
      fal_console_move_to(console, x, y);
      char c = ' ';
      if ((x == x1 && y == y1) || (x == x2 && y == y1) ||
          (x == x1 && y == y2) || (x == x2 && y == y2)) {
        c = border_type == FAL_FRAME_BORDER_SINGLE ? '+' : '#';
      } else if (x == x1 || x == x2) {
        c = border_type == FAL_FRAME_BORDER_SINGLE ? '|' : '#';
      } else if (y == y1 || y == y2) {
        c = border_type == FAL_FRAME_BORDER_SINGLE ? '-' : '=';
      }

      printf("%c", c);
    }
  }
}

void fal_console_write_at(fal_console *console, int x, int y,
                          const char *text) {
  fal_console_move_to(console, x, y);
  int cur_x = x;
  int cur_y = y;
  size_t text_length = strlen(text);
  for (int i = 0; i < text_length; i++) {
    printf("%c", text[i]);
    if (cur_x >= FAL_CONSOLE_WIDTH - 1) {
      cur_x = 0;
      cur_y++;
      fal_console_move_to(console, cur_x, cur_y);
    } else {
      cur_x++;
    }

    if (cur_y > FAL_CONSOLE_HEIGHT) {
      break;
    }
  }
}
