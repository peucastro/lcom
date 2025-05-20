#include "controller/graphics/graphics.h"
#include "model/menu/menu.h"

extern int current_option;

void draw_menu(void) {
  graphics_clear_screen();
  graphics_draw_rectangle(0, 0, 1152, 864, 0xFF0000);
}
