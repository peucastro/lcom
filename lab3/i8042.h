#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define MAKE_CODE BIT(7)

#define KBC_OUT 0X60

#define KBC_IN 0X64

#define KBC_ST 0X64

#define KBC_PARITY_ERROR BIT(7)

#define KBC_TIMEOUT_ERROR BIT(6)

#define KBC_FULL_IBF BIT(1)

#define KBC_FULL_OBF BIT(0)

#endif /* _LCOM_I8042_H_ */
