#include "platform.h"
#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static enum fal_key fal_key_from_char(char c) {
  if (c == 27) {
    return FAL_KEY_ESCAPE;
  }

  switch (toupper(c)) {
  case 'A':
    return FAL_KEY_A;
  case 'B':
    return FAL_KEY_B;
  case 'C':
    return FAL_KEY_C;
  case 'D':
    return FAL_KEY_D;
  case 'E':
    return FAL_KEY_E;
  case 'F':
    return FAL_KEY_F;
  case 'G':
    return FAL_KEY_G;
  case 'H':
    return FAL_KEY_H;
  case 'I':
    return FAL_KEY_I;
  case 'J':
    return FAL_KEY_J;
  case 'K':
    return FAL_KEY_K;
  case 'L':
    return FAL_KEY_L;
  case 'M':
    return FAL_KEY_M;
  case 'N':
    return FAL_KEY_N;
  case 'O':
    return FAL_KEY_O;
  case 'P':
    return FAL_KEY_P;
  case 'Q':
    return FAL_KEY_Q;
  case 'R':
    return FAL_KEY_R;
  case 'S':
    return FAL_KEY_S;
  case 'T':
    return FAL_KEY_T;
  case 'U':
    return FAL_KEY_U;
  case 'V':
    return FAL_KEY_V;
  case 'W':
    return FAL_KEY_W;
  case 'X':
    return FAL_KEY_X;
  case 'Y':
    return FAL_KEY_Y;
  case 'Z':
    return FAL_KEY_Z;
  default:
    return FAL_KEY_UNKNOWN;
  }
}

enum fal_key fal_platform_read_key(void) {
  struct termios oldt, newt;
  char ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return fal_key_from_char(ch);
}
