#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>

#include "i8042.h"

int(kbc_ready)(uint8_t *st);

int(kbc_read_st)(uint8_t *st);

int(kbc_read_buffer)(uint8_t *data);

int(kbc_write_cmd)(int port, uint8_t cmd);

#endif /* __KBC_H */
