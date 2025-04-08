#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

/** @defgroup vbe vbe
 * @{
 *
 * Constants for using the Video Electronics Standards Association (VESA) Basic Input/Output System (BIOS) Extensions (VBE)
 */

#define VBE_INT 0x10

/* VBE functions */

#define VBE_FUNCTION 0x4F

#define VBE_SET_VIDEO_MODE 0X00

/* VBE_FUNCTION arguments */

#define VBE_RET_CTRL 0x00

#define VBE_RET_MODE 0x01

#define VBE_SET_MODE 0x02

/* VBE functions return values */

#define VBE_CALL_SUCCESS 0x00

#define VBE_CALL_FAIL 0x01

#define VBE_CALL_NOT_SUPPORTED 0x02

#define VBE_CALL_INVALID 0x03

/* VBE modes */

#define VBE_MODE_640x480 0x110

#define VBE_MODE_800x600 0x115

#define VBE_MODE_1152x864 0x14C

#define VBE_MODE_1024x768 0x105

#define VBE_MODE_1280x1024 0x11A

#define VBE_LINEAR_MODE BIT(14)

#endif
