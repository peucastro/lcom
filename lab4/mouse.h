#ifndef __MOUSE_H
#define __MOUSE_H

#include "kbc.h"

/** @defgroup mouse mouse
 * @{
 *
 * Functions for interacting with the Mouse.
 */

/**
 * @brief Getter for the mouse index
 *
 * @return Return the current mouse index
 */
uint8_t(mouse_get_index)(void);

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

/**
 * @brief Synchronizes the driver with the mouse packet stream
 */
void(mouse_sync)(void);

/**
 * @brief Parses a mouse packet accoarding to the documentation
 *
 * @return Returns the parsed struct mouse packet
 */
struct packet(mouse_parse_packet)(void);

/**
 * @brief Mouse interrupt handler
 *
 * Reads the byte from the KBC output buffer
 */
void(mouse_ih)(void);

/**@}*/

#endif /* __MOUSE_H */
