#include "console.h"
#include "js.h"

void fal_console_render_init(void) { js_initialize_canvas(); }
void fal_console_render_deinit(void) {}

void fal_console_render(
    struct fal_console_char buf[FAL_CONSOLE_HEIGHT][FAL_CONSOLE_WIDTH]) {
  js_draw_console(&buf[0][0], FAL_CONSOLE_WIDTH, FAL_CONSOLE_HEIGHT);
}
