#ifndef __PROJ_RESOURCES_H
#define __PROJ_RESOURCES_H

#include "model/sprite/sprite.h"

/** @defgroup resources resources
 * @{
 *
 * Functions for managing game resources
 */

/**
 * @brief Creates all game resources
 *
 * @return 0 upon success, non-zero otherwise
 */
int(create_resources)();

/**
 * @brief Destroys all game resources
 */
void(destroy_resources)();

/**@}*/

#endif /* __PROJ_RESOURCES_H */
