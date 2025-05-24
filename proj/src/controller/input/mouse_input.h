#ifndef __MOUSE_INPUT_H
#define __MOUSE_INPUT_H

#include "model/game/game.h"
#include "controller/kbc/mouse.h"

/* button‐hit rectangles */
#define START_BX  243   /* left edge of “Start” */
#define START_BY  541   /* top  of both buttons */
#define BUTTON_W  230   /* width of “Start” */
#define BUTTON_H   35   /* height of both buttons    */

#define EXIT_BX   617   /* left edge of “Exit”  */
#define EXIT_BY   541   /* same top Y as “Start”*/

void handle_mouse_input(Game *game);

void handle_menu_mouse(Game *game);

#endif
