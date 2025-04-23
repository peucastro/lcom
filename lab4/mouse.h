#ifndef __MOUSE_H
#define __MOUSE_H

#include "i8042.h"
#include "kbc.h"

/**
 * @brief Getter for the mouse index
 *
 * @return Return the current mouse index
 */
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

/**
 * @brief Writes a command to the mouse
 *
 * Sends a command to the mouse by writing the appropriate bytes to the KBC.
 * Retries up to a maximum number of attempts if the mouse does not acknowledge the command.
 *
 * @param cmd The command to be sent to the mouse
 * @return Return 0 upon success, or 1 if an error occurs or the mouse does not acknowledge the command
 */
int(mouse_write_cmd)(uint8_t cmd);

void(mouse_sync)(void);

struct packet(mouse_parse_packet)(void);

void(mouse_ih)(void);

#endif /* __MOUSE_H */
