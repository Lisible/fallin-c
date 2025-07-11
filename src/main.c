#include "console.h"

int main(void) {
  struct fal_console console = {};
  fal_console_init(&console);
  fal_console_clear(&console,
                    &(const struct fal_clear_desc){.color = FAL_COLOR_BLUE,
                                                   .update_background = true});
  fal_console_draw_rect(&console, (struct fal_position){0, 0},
                        (struct fal_position){5, 5});
  fal_console_write_at(&console, (struct fal_position){10, 10},
                       "dfslfjklsdjfglsjgflsdjglkdfgjldfjglkrejbvlejr;vbker;"
                       "lbmerge;rlgke;rgkd;lfkg;ldfkg;dlkgj;fdlgj;dlgj");
  fal_console_render(console.content);

  fal_console_deinit(&console);

  return 0;
}
