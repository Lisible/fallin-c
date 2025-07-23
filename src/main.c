#include "console.h"

static void fal_main(void) {
  struct fal_console console = {};
  fal_console_init(&console);
  fal_console_clear(&console,
                    &(const struct fal_clear_desc){.color = FAL_COLOR_BLUE,
                                                   .update_background = true});
  fal_console_draw_frame(&console, &(const struct fal_console_frame_infos){
                                       .top_left = {1, 1},
                                       .bottom_right = {6, 6},
                                       .background_color = FAL_COLOR_YELLOW,
                                       .text_color = FAL_COLOR_WHITE,
                                       .border_type = FAL_FRAME_BORDER_SINGLE});
  fal_console_set_bg_color(&console, FAL_COLOR_BLACK);
  fal_console_write_at(&console, (struct fal_position){10, 10},
                       "dfslfjklsdjfglsjgflsdjglkdfgjldfjglkrejbvlejr;vbker;"
                       "lbmerge;rlgke;rgkd;lfkg;ldfkg;dlkgj;fdlgj;dlgj");
  fal_console_render(console.content);

  fal_console_deinit(&console);
}

#ifdef FAL_WASM
#include "js.h"
int wasm_main(void) {
  js_log("Startup");
  fal_main();
  return 0;
}
#else
int main(void) {
  fal_main();
  return 0;
}
#endif // FAL_WASM
