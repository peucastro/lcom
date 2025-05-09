#ifndef __PROJ_IH
#define __PROJ_IH

#include "controller/kbc/kbd.h"
#include "controller/kbc/mouse.h"
#include "controller/timer/timer.h"

/** @defgroup ih_handler ih_handler
 * @{
 *
 * Functions for using the interrupt handlers
 */

/**
 * @brief Subscribes for all the devices interrups.
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int(subscribe_interrupts)(void);

/**
 * @brief Unsubscribes for all the devices interrups.
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int(unsubscribe_interrupts)(void);

/**
 * @brief Processes device interrupts based on the interrupt bitmask
 *
 * @param irq_mask Bitmask containing the pending interrupt flags
 */
void(process_interrupts)(uint64_t irq_mask);

#endif /* __PROJ_IH */
