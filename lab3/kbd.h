#ifndef __KBD_H
#define __KBD_H

#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

/**
 * @brief Subscribes keyboard interrupts
 *
 * @param bit_no Address of memory position to be filled with the hook_id
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_unsubscribe_int)(void);

/**
 * @brief Enables keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_enable_int)(void);

/**
 * @brief Keyboard interrupt handler
 *
 * Reads the scancode from the KBC output buffer
 */
void(kbc_ih)(void);

#endif /* __KBD_H */
