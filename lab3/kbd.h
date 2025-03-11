#ifndef __KBD_H
#define __KBD_H

#include "kbc.h"
#include <stdint.h>

#define KBD_IRQ 1

int(kbd_subscribe_int)(uint8_t *bit_no);

int(kbd_unsubscribe_int)();

void(kbc_ih)();

#endif /* __KBD_H */
