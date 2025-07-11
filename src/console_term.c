#include "console.h"
#include "panic.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static unsigned char bg_color(fal_color color) {
  static const int BG_COLOR_OFFSET = 10;
  return color + BG_COLOR_OFFSET;
}

static struct termios orig_termios;
void fal_console_render_init(void) {
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
}
void fal_console_render_deinit(void) {
  // TODO error handling
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) != 0) {
    FAL_PANIC("Couldn't set terminal parameter informations: %s",
              strerror(errno));
  }
  printf("\033[0m");
}

void fal_console_render(
    struct fal_console_char buf[FAL_CONSOLE_HEIGHT][FAL_CONSOLE_WIDTH]) {
  printf("\033[2J");
  printf("\033[H");

  for (int y = 0; y < FAL_CONSOLE_HEIGHT; y++) {
    for (int x = 0; x < FAL_CONSOLE_WIDTH; x++) {
      printf("\033[%d;%dm%c", bg_color(buf[y][x].bg), buf[y][x].fg,
             buf[y][x].character);
    }
    printf("\n\r");
  }
  printf("\n\r");
}
