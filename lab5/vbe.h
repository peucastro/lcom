#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/** @defgroup vbe vbe
 * @{
 *
 * Constants and functions for using the Video Electronics Standards Association (VESA) Basic Input/Output System (BIOS) Extensions (VBE)
 */

#define VBE_INT 0x10 /**< @brief BIOS interrupt for VBE functions */

/* VBE functions */

#define VBE_FUNCTION 0x4F       /**< @brief VBE function identifier */
#define VBE_SET_VIDEO_MODE 0x00 /**< @brief VBE function to set video mode */

/* VBE_FUNCTION arguments */

#define VBE_RET_CTRL 0x00 /**< @brief VBE function to return controller information */
#define VBE_RET_MODE 0x01 /**< @brief VBE function to return mode information */
#define VBE_SET_MODE 0x02 /**< @brief VBE function to set a video mode */

/* VBE functions return values */

#define VBE_CALL_SUCCESS 0x00       /**< @brief VBE call completed successfully */
#define VBE_CALL_FAIL 0x01          /**< @brief VBE call failed */
#define VBE_CALL_NOT_SUPPORTED 0x02 /**< @brief VBE function not supported in current hardware configuration */
#define VBE_CALL_INVALID 0x03       /**< @brief VBE function invalid in current video mode */

/* VBE modes */

#define VBE_MODE_640x480 0x110   /**< @brief 640x480 resolution, 15-bit direct color (1:5:5:5) */
#define VBE_MODE_800x600 0x115   /**< @brief 800x600 resolution, 24-bit direct color (8:8:8) */
#define VBE_MODE_1024x768 0x105  /**< @brief 1024x768 resolution, 8-bit indexed color */
#define VBE_MODE_1280x1024 0x11A /**< @brief 1280x1024 resolution, 16-bit direct color (5:6:5) */
#define VBE_MODE_1152x864 0x14C  /**< @brief 1152x864 resolution, 32-bit direct color (8:8:8:8) */
#define VBE_LINEAR_MODE BIT(14)  /**< @brief Linear frame buffer mode bit */

/* Functions for using the VBE */

vbe_mode_info_t(vbe_get_mode)(void);

uint8_t *(vbe_get_video_mem) (void);

/**
 * @brief Initializes the video card in graphics mode.
 *
 * @param mode 16-bit VBE mode to set
 * @return Return 0 upon sucess, non-zero otherwise
 */
int(vbe_set_video_mode)(uint16_t mode);

/**
 * @brief Maps the video card VRAM to the program's address space.
 *
 * @param mode 16-bit VBE mode to set
 * @return Return 0 upon success, non-zero otherwise
 */
int(vbe_map_vram)(uint16_t mode);

/**@}*/

#endif /* _LCOM_VBE_H_ */
