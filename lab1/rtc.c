#include "rtc.h"

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_UIP_MSK (1 << 7)
#define RTC_DM_MSK (1 << 2)

static int bcd_to_bin(uint8_t bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

int rtc_read_date(rtc_date *date) {
  if (date == NULL) {
    fprintf(stderr, "rtc_read_date: date pointer cannot be null.\n");
    return 1;
  }

  uint32_t reg_a, reg_b, day, month, year;

  do {
    if (sys_outb(RTC_ADDR_REG, RTC_REG_A) != 0) {
      perror("rtc_read_date: failed to write to RTC address register (Reg A)");
      return 1;
    }
    if (sys_inb(RTC_DATA_REG, &reg_a) != 0) {
      perror("rtc_read_date: failed to read from RTC data register (Reg A)");
      return 1;
    }
  } while (reg_a & RTC_UIP_MSK);

  if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != 0) {
    perror("rtc_read_date: failed to write to RTC address register (Reg B)");
    return 1;
  }
  if (sys_inb(RTC_DATA_REG, &reg_b) != 0) {
    perror("rtc_read_date: failed to read from RTC data register (Reg B)");
    return 1;
  }

  int is_bcd = !(reg_b & RTC_DM_MSK);

  if (sys_outb(RTC_ADDR_REG, RTC_REG_DAY) != 0 || sys_inb(RTC_DATA_REG, &day) != 0) {
    fprintf(stderr, "rtc_read_date: failed to read Day register.\n");
    return 1;
  }
  if (sys_outb(RTC_ADDR_REG, RTC_REG_MONTH) != 0 || sys_inb(RTC_DATA_REG, &month) != 0) {
    fprintf(stderr, "rtc_read_date: failed to read Month register.\n");
    return 1;
  }
  if (sys_outb(RTC_ADDR_REG, RTC_REG_YEAR) != 0 || sys_inb(RTC_DATA_REG, &year) != 0) {
    fprintf(stderr, "rtc_read_date: failed to read Year register.\n");
    return 1;
  }

  date->day = is_bcd ? (uint8_t) bcd_to_bin(day) : (uint8_t) day;
  date->month = is_bcd ? (uint8_t) bcd_to_bin(month) : (uint8_t) month;
  date->year = is_bcd ? (uint8_t) bcd_to_bin(year) : (uint8_t) year;

  return 0;
}
