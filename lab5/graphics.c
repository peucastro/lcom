#include <lcom/lcf.h>

#include "graphics.h"

static vbe_mode_info_t mode_info;
static uint8_t *video_mem;

int(set_video_mode)(uint16_t mode) {
  struct reg86 args;
  memset(&args, 0, sizeof(args));

  args.ah = VBE_FUNCTION;
  args.al = VBE_SET_MODE;
  args.bx = mode | VBE_LINEAR_MODE;
  args.intno = VBE_INT;

  if (sys_int86(&args) != 0) {
    perror("set_video_mode: failed to call sys_int86.");
    return 1;
  }

  if (args.ah != VBE_CALL_SUCCESS || args.al != VBE_FUNCTION) {
    if (args.ah == VBE_CALL_FAIL) {
      perror("set_video_mode: vbe call fail.");
    }
    if (args.ah == VBE_CALL_NOT_SUPPORTED) {
      perror("set_video_mode: vbe call not supported.");
    }
    if (args.ah == VBE_CALL_INVALID) {
      perror("set_video_mode: vbe call invalid.");
    }
    return 1;
  }

  return 0;
}

int(map_graphics_vram)(uint16_t mode) {
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    perror("map_graphics_vram: failed to get mode info.");
    return 1;
  }

  struct minix_mem_range mr;
  memset(&mr, 0, sizeof(mr));
  mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + mode_info.XResolution * mode_info.YResolution * mode_info.BitsPerPixel;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
    perror("map_graphics_vram: failed to request permission.");
    return 1;
  }

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, mr.mr_limit - mr.mr_base);

  return 0;
}
