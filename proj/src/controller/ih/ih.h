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
 * @brief Handles timer interrupts
 *
 * Calls the timer interrupt handler and processes any related events.
 */
void(timer_handler)(void);

/**
 * @brief Handles keyboard interrupts
 *
 * Reads scan codes from the keyboard controller and processes keyboard events.
 */
void(kbd_handler)(void);

/**
 * @brief Handles mouse interrupts
 *
 * Processes mouse data packets and handles mouse-related events.
 */
void(mouse_handler)(void);

/**
 * @brief Processes device interrupts based on the interrupt bitmask
 *
 * @param irq_mask Bitmask containing the pending interrupt flags
 */
void(process_interrupts)(uint64_t irq_mask);

#endif /* __PROJ_IH */
