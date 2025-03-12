#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>

#include "i8042.h"

int(read_kbc_st)(uint8_t *st);

int(read_kbc_data)(uint8_t *data);

#endif /* __KBC_H */
