#include <lcom/lcf.h>

#include "vbe.h"

/* static global variable to store vbe mode information.
 * this structure holds details about the selected video mode */
static vbe_mode_info_t mode_info;
/* static global pointer to the mapped video memory in the process's address space.
 * in minix 3, vram is not directly accessible and needs to be mapped */
static uint8_t *video_mem;

vbe_mode_info_t(vbe_get_mode)(void) {
  return mode_info;
}

uint8_t *(vbe_get_video_mem) (void) {
  return video_mem;
}

int(vbe_set_video_mode)(uint16_t mode) {
  // holds the arguments for the bios interrupt call
  struct reg86 args;
  // clear the reg86 structure to avoid unexpected behavior
  if (memset(&args, 0, sizeof(args)) == NULL) {
    perror("vbe_set_video_mode: failed to clear reg86.");
    return 1;
  }

  // set ah register to VBE_FUNCTION (0x4f) to indicate a vbe function call
  args.ah = VBE_FUNCTION;
  // set al register to VBE_SET_MODE (0x02) to select the "set vbe mode" function
  args.al = VBE_SET_MODE;
  /* set bx register to the desired video mode number ored with vbe_linear_mode.
   * setting bit 14 of bx enables the linear frame buffer model for easier vram access */
  args.bx = mode | VBE_LINEAR_MODE;
  // set intno to VBE_INT (0x10) to specify the bios video services interrupt
  args.intno = VBE_INT;

  // call sys_int86 to invoke the bios interrupt with the specified register values
  if (sys_int86(&args) != 0) {
    perror("vbe_set_video_mode: failed to call sys_int86.");
    return 1;
  }

  /* check the return status in ah and al registers after the vbe call.
   * al should be 0x4f if the vbe function is supported, and ah should be 0x00 for success */
  if (args.ah != VBE_CALL_SUCCESS || args.al != VBE_FUNCTION) {
    // check if ah indicates a failed function call (0x01).
    if (args.ah == VBE_CALL_FAIL) {
      fprintf(stderr, "vbe_set_video_mode: vbe call fail.");
    }
    // check if ah indicates the function is not supported in the current hardware configuration (0x02)
    else if (args.ah == VBE_CALL_NOT_SUPPORTED) {
      fprintf(stderr, "vbe_set_video_mode: vbe call not supported.");
    }
    // check if ah indicates the function is invalid in the current video mode (0x03)
    else if (args.ah == VBE_CALL_INVALID) {
      fprintf(stderr, "vbe_set_video_mode: vbe call invalid.");
    }
    // handle other non-success return codes in ah
    else {
      fprintf(stderr, "vbe_set_video_mode: function not supported.");
    }
    return 1;
  }

  return 0;
}

int(vbe_map_vram)(uint16_t mode) {
  /* check if setting the memory of the mode_info structure to zero fails.
   * mode_info will store the video mode information */
  if (memset(&mode_info, 0, sizeof(mode_info)) == NULL) {
    perror("vbe_map_vram: failed to clear mode_info.");
    return 1;
  }
  /* call the vbe_get_mode_info function to retrieve information about the specified video mode.
   * this function is provided by the lcf */
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    fprintf(stderr, "vbe_map_vram: failed to get mode info.");
    return 1;
  }

  // declare a struct minix_mem_range to define the physical memory range
  struct minix_mem_range mr;
  // check if setting the memory of the minix_mem_range structure to zero fails
  if (memset(&mr, 0, sizeof(mr)) == NULL) {
    perror("vbe_map_vram: failed to clear minix_mem_range.");
    return 1;
  }
  // set the base physical address of the memory range to the physical base pointer obtained from mode_info.
  mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;
  // set the limit of the memory range to the base address plus the total vram size.
  mr.mr_limit = mr.mr_base + mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel + 7) / 8;

  // request permission to map the specified physical memory range into the process's address space using the sys_privctl kernel call
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
    perror("vbe_map_vram: failed to request permission.");
    return 1;
  }

  // map the physical memory region into the process's virtual address space using the vm_map_phys kernel call.
  video_mem = (uint8_t *) vm_map_phys(SELF, (void *) mr.mr_base, mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel + 7) / 8);

  // check if the memory mapping failed.
  if ((void *) video_mem == MAP_FAILED) {
    fprintf(stderr, "vbe_map_vram: map failed.");
    return 1;
  }

  return 0;
}
