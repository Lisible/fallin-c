#ifndef FAL_PLATFORM_H
#define FAL_PLATFORM_H

enum fal_key {
  FAL_KEY_A = 0,
  FAL_KEY_B,
  FAL_KEY_C,
  FAL_KEY_D,
  FAL_KEY_E,
  FAL_KEY_F,
  FAL_KEY_G,
  FAL_KEY_H,
  FAL_KEY_I,
  FAL_KEY_J,
  FAL_KEY_K,
  FAL_KEY_L,
  FAL_KEY_M,
  FAL_KEY_N,
  FAL_KEY_O,
  FAL_KEY_P,
  FAL_KEY_Q,
  FAL_KEY_R,
  FAL_KEY_S,
  FAL_KEY_T,
  FAL_KEY_U,
  FAL_KEY_V,
  FAL_KEY_W,
  FAL_KEY_X,
  FAL_KEY_Y,
  FAL_KEY_Z,
  FAL_KEY_ESCAPE,
  FAL_KEY_COUNT,
  FAL_KEY_UNKNOWN,
};
enum fal_key fal_key_from_string(const char *key);

enum fal_key fal_platform_read_key(void);

#endif // FAL_PLATFORM_H
