#ifndef FAL_JS
#define FAL_JS

#include "console.h"

void js_log(const char *message);
void js_initialize_canvas(void);
void js_draw_console(struct fal_console_char *buf, int width, int height);

#endif // FAL_JS
