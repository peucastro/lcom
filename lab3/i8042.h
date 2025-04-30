#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller.
 */

#define KBD_IRQ 1          /**< @brief Keyboard IRQ line */
#define KBC_MAX_ATTEMPTS 5 /**< @brief Maximum amount of attempts to read KBC data */
#define DELAY_US 20000     /**< @brief KBC response time */

/* I/O port addresses */

#define KBC_OUT 0X60 /**< @brief KBC output buffer port */
#define KBC_IN 0X64  /**< @brief KBC input buffer port */
#define KBC_ST 0X64  /**< @brief KBC status register port */
#define KBC_ARG 0x60 /**< @brief KBC arguments input buffer */

/* Scancode-relaed */

#define MAKE_CODE BIT(7) /**< @brief Make code bit mask */
#define CODE_HEADER 0xE0 /**< @brief Byte that indicates if the scancode has 2 bytes */
#define BREAK_ESC 0x81   /**< @brief ESC key breakcode */

/* Status byte format */

#define KBC_PARITY_ERROR BIT(7)  /**< @brief Parity error bit mask in status register */
#define KBC_TIMEOUT_ERROR BIT(6) /**< @brief Timeout error bit mask in status register */
#define KBC_AUX BIT(5)           /**< @brief Mouse data */
#define KBC_INH BIT(4)           /**< @brief Inhibit flag */
#define KBC_FULL_IBF BIT(1)      /**< @brief Input buffer full bit mask in status register */
#define KBC_FULL_OBF BIT(0)      /**< @brief Output buffer full bit mask in status register */

/* Keyboard-Related KBC Commands for PC-AT/PS2 */

#define KBC_READ_CMD 0x20  /**< @brief Read Command Byte */
#define KBC_WRITE_CMD 0x60 /**< @brief Write Command Byte */
#define KBC_CHK 0xAA       /**< @brief KBC command to self-check */
#define KBC_CHK_KBD_I 0xAB /**< @brief KBC command to check keyboard interface */
#define KBC_DIS_KBD_I 0XAD /**< @brief KBC command to disable KBD interface */
#define KBC_EN_KBD_I 0XAE  /**< @brief KBC command to enable KBD interface */

/* KBC Command Byte */

#define KBC_DIS2 BIT(5) /**< @brief Disable mouse interface */
#define KBC_DIS BIT(4)  /**< @brief Disable keyboard interface */
#define KBC_INT2 BIT(1) /**< @brief Enable interrupt on OBF, from mouse */
#define KBC_INT BIT(0)  /**< @brief Enable interrupt on OBF, from keyboard */

/**@}*/

#endif /* _LCOM_I8042_H_ */
