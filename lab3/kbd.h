#ifndef __KBD_H
#define __KBD_H

#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

int(kbd_subscribe_int)(uint8_t *bit_no);

int(kbd_unsubscribe_int)();

int(kbd_enable_int)();

void(kbc_ih)();

#endif /* __KBD_H */
