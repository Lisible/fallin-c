#include "engine.h"

int main(void) {
  struct fal_engine *engine = fal_engine_create();
  fal_engine_destroy(engine);
  // fal_console *console = fal_console_create();
  // fal_console_set_background_color(console, FAL_COLOR_BLUE);
  // fal_console_set_text_color(console, FAL_COLOR_RED);
  // fal_console_clear(console);
  // fal_console_write_at(console, 10, 5,
  //                      "Bonjour les amis "
  //                      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  //                      "aaaaaaaaaaaaaaaaaaaaaaaaa");
  // fal_console_draw_frame(console, 15, 3, 79, 24, FAL_FRAME_BORDER_SINGLE,
  //                        FAL_COLOR_YELLOW, FAL_COLOR_WHITE);
  // fal_console_draw_frame(console, 0, 0, 0, 0, FAL_FRAME_BORDER_SINGLE,
  //                        FAL_COLOR_RED, FAL_COLOR_RED);
  // fal_console_draw_frame(console, 79, 24, 79, 24, FAL_FRAME_BORDER_SINGLE,
  //                        FAL_COLOR_RED, FAL_COLOR_RED);
  // fal_console_destroy(console);

  // while (fal_platform_read_key() != 27)
  //   ;
  return 0;
}
