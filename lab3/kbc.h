#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>

#include "i8042.h"

int(read_kbc_st)(uint8_t *st);

int(read_kbc_data)(uint8_t port, uint8_t *data);

int(kbc_ready)(uint8_t *st);

int(kbc_write_cmd)(int port, uint8_t cmd);

#endif /* __KBC_H */
