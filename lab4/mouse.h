#ifndef __MOUSE_H
#define __MOUSE_H

#include "i8042.h"
#include "kbc.h"

uint8_t(mouse_get_index)();

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

int(mouse_write_cmd)(uint8_t cmd);

void(mouse_sync)(void);

struct packet(mouse_parse_packet)(void);

void(mouse_ih)(void);

#endif /* __MOUSE_H */
