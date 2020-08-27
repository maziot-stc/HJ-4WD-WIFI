#ifndef __DS1302__
#define __DS1302__

#include "reg52.h"

#define low   0
#define high  1

sbit DS1302_SCK = P1 ^ 0;
sbit DS1302_IO  = P1 ^ 1;
sbit DS1302_RST = P1 ^ 2;

#define DS1302_ADDR_YEAR     0x8C
#define DS1302_ADDR_WEEK     0x8A
#define DS1302_ADDR_MONTH    0x88
#define DS1302_ADDR_DAY      0x86
#define DS1302_ADDR_HOUR     0x84
#define DS1302_ADDR_MINUTE   0x82
#define DS1302_ADDR_SECOND   0x80

/**
 * 日期时间结构体
 */
typedef struct __DS1302_DATE_TIME__
{
  unsigned char year;
  unsigned char week;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
} ds1302_Datetime;

unsigned char to_bcd(unsigned char val);
unsigned char from_bcd(unsigned char bcd);
void ds1302_write(unsigned char dat);
void ds1302_write_to_addr(unsigned char addr, unsigned char dat);
unsigned char ds1302_read_from_addr(unsigned char addr);

void ds1302_set_datetime(ds1302_Datetime *datetime);
void ds1302_get_datetime(ds1302_Datetime *datetime);

#endif  // __DS1302__
