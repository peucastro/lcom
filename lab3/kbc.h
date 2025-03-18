#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>

#include "i8042.h"

/** @defgroup kbd kbc
 * @{
 *
 * Functions for using the i8042 Keyboard Controller.
 */

/**
 * @brief Checks if the KBC is ready
 *
 * @param st The KBC status byte
 * @return Return 0 if ready, non-zero otherwise
 */
int(kbc_ready)(uint8_t st);

/**
 * @brief Reads the status from the KBC
 *
 * @param st Address of memory position to be filled with the status
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_st)(uint8_t *st);

/**
 * @brief Reads the buffer from the KBC
 *
 * @param port Port to read from
 * @param data Address of memory position to be filled with the data
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_buffer)(uint8_t port, uint8_t *data);

/**
 * @brief Reads data from the KBC
 *
 * @param data Address of memory position to be filled with the data
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_data)(uint8_t *data);

/**
 * @brief Writes a command to the KBC
 *
 * @param port Port to write to
 * @param cmd Command to write
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_write_cmd)(int port, uint8_t cmd);

#endif /* __KBC_H */
