#ifndef __MOUSE_H
#define __MOUSE_H

#include "i8042.h"

/**
 * @brief Subscribes mouse interrupts
 *
 * @param bit_no Address of memory position to be filled with the hook_id
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_unsubscribe_int)(void);

#endif /* __MOUSE_H */
