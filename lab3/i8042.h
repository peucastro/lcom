#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KBD_IRQ 1 /**< @brief Keyboard IRQ line */

/* I/O port addresses */

#define KBC_OUT 0X60 /**< @brief KBC output buffer port */

#define KBC_IN 0X64 /**< @brief KBC input buffer port */

#define KBC_ST 0X64 /**< @brief KBC status register port */

/* Scancode-relaed */

#define MAKE_CODE BIT(7) /**< @brief Make code bit mask */

#define CODE_HEADER 0xE0 /**< @brief Byte that indicates if the scancode has 2 bytes */

#define BREAK_ESC 0x81 /**< @brief ESC key breakcode */

/* Status byte format */

#define KBC_PARITY_ERROR BIT(7) /**< @brief Parity error bit mask in status register */

#define KBC_TIMEOUT_ERROR BIT(6) /**< @brief Timeout error bit mask in status register */

#define KBC_FULL_IBF BIT(1) /**< @brief Input buffer full bit mask in status register */

#define KBC_FULL_OBF BIT(0) /**< @brief Output buffer full bit mask in status register */

/* Keyboard-Related KBC Commands for PC-AT/PS2 */

#define KBC_READ_CMD 0x20  /**< @brief Read Command Byte */

#define KBC_WRITE_CMD 0x60 /**< @brief Write Command Byte */

#endif /* _LCOM_I8042_H_ */
