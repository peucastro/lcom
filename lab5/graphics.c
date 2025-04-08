#include <lcom/lcf.h>

#include "graphics.h"

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
