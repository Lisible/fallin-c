#include "platform.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
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
enum fal_key fal_key_from_string(const char *key) {

  if (strcmp("A", key) == 0 || strcmp("a", key) == 0) {
    return FAL_KEY_A;
  } else if (strcmp("B", key) == 0 || strcmp("b", key) == 0) {
    return FAL_KEY_B;
  } else if (strcmp("C", key) == 0 || strcmp("c", key) == 0) {
    return FAL_KEY_C;
  } else if (strcmp("D", key) == 0 || strcmp("d", key) == 0) {
    return FAL_KEY_D;
  } else if (strcmp("E", key) == 0 || strcmp("e", key) == 0) {
    return FAL_KEY_E;
  } else if (strcmp("F", key) == 0 || strcmp("f", key) == 0) {
    return FAL_KEY_F;
  } else if (strcmp("G", key) == 0 || strcmp("g", key) == 0) {
    return FAL_KEY_G;
  } else if (strcmp("H", key) == 0 || strcmp("h", key) == 0) {
    return FAL_KEY_H;
  } else if (strcmp("I", key) == 0 || strcmp("i", key) == 0) {
    return FAL_KEY_I;
  } else if (strcmp("J", key) == 0 || strcmp("j", key) == 0) {
    return FAL_KEY_J;
  } else if (strcmp("K", key) == 0 || strcmp("k", key) == 0) {
    return FAL_KEY_K;
  } else if (strcmp("L", key) == 0 || strcmp("l", key) == 0) {
    return FAL_KEY_L;
  } else if (strcmp("M", key) == 0 || strcmp("m", key) == 0) {
    return FAL_KEY_M;
  } else if (strcmp("N", key) == 0 || strcmp("n", key) == 0) {
    return FAL_KEY_N;
  } else if (strcmp("O", key) == 0 || strcmp("o", key) == 0) {
    return FAL_KEY_O;
  } else if (strcmp("P", key) == 0 || strcmp("p", key) == 0) {
    return FAL_KEY_P;
  } else if (strcmp("Q", key) == 0 || strcmp("q", key) == 0) {
    return FAL_KEY_Q;
  } else if (strcmp("R", key) == 0 || strcmp("r", key) == 0) {
    return FAL_KEY_R;
  } else if (strcmp("S", key) == 0 || strcmp("s", key) == 0) {
    return FAL_KEY_S;
  } else if (strcmp("T", key) == 0 || strcmp("t", key) == 0) {
    return FAL_KEY_T;
  } else if (strcmp("U", key) == 0 || strcmp("u", key) == 0) {
    return FAL_KEY_U;
  } else if (strcmp("V", key) == 0 || strcmp("v", key) == 0) {
    return FAL_KEY_V;
  } else if (strcmp("W", key) == 0 || strcmp("w", key) == 0) {
    return FAL_KEY_W;
  } else if (strcmp("X", key) == 0 || strcmp("x", key) == 0) {
    return FAL_KEY_X;
  } else if (strcmp("Y", key) == 0 || strcmp("y", key) == 0) {
    return FAL_KEY_Y;
  } else if (strcmp("Z", key) == 0 || strcmp("z", key) == 0) {
    return FAL_KEY_Z;
  } else if (strcmp("escape", key) == 0) {
    return FAL_KEY_ESCAPE;
  } else {
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
