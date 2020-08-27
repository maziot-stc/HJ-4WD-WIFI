#include "bsp_ds1302z.h"

unsigned char to_bcd(unsigned char val)
{
  return ((val / 10) << 4 | (val % 10));
}

unsigned char from_bcd(unsigned char bcd)
{
  return ((bcd&0x70)>>4)*10 + (bcd&0x0F);
}

/**
 * ds1302
 * @param dat 数据
 */
void ds1302_write(unsigned char dat)
{
  unsigned char i;
  for(i=0; i<8; i++)
  {
    DS1302_SCK = low;
    DS1302_IO = dat & 0x01;
    dat >>= 1;
    DS1302_SCK = high;
  }
}

/**
 * 向指定地址写入数据
 * @param addr 地址
 * @param dat 数据
 */
void ds1302_write_to_addr(unsigned char addr, unsigned char dat)
{
  DS1302_RST = low;
  DS1302_SCK = low;
  DS1302_RST = high;
  ds1302_write(addr);
  ds1302_write(dat);
  // 释放
  DS1302_RST = low;
  DS1302_IO = high;
  DS1302_SCK = high;
}

/**
 * 从指定地址读取数据
 * @param  addr [description]
 * @return      [description]
 */
unsigned char ds1302_read_from_addr(unsigned char addr)
{
  unsigned char i, value = 0;
  DS1302_RST = low;
  DS1302_SCK = low;
  DS1302_RST = high;
  ds1302_write(addr);
  for(i=0; i<8; i++)
  {
    DS1302_SCK = low;
    value >>= 1;
    if(DS1302_IO)
      value |= 0x80;
    DS1302_SCK = high;
  }
  // 释放
  DS1302_RST = low;
  DS1302_IO = high;
  DS1302_SCK = high;
  return value;
}

/**
 * 设置1302
 * @param attr  [description]
 * @param value [description]
 */
void ds1302_set_datetime(ds1302_Datetime *datetime)
{
  ds1302_write_to_addr(0x8E, 0x00);
  ds1302_write_to_addr(DS1302_ADDR_YEAR, to_bcd(datetime->year));
  ds1302_write_to_addr(DS1302_ADDR_WEEK, to_bcd(datetime->week));
  ds1302_write_to_addr(DS1302_ADDR_MONTH, to_bcd(datetime->month));
  ds1302_write_to_addr(DS1302_ADDR_DAY, to_bcd(datetime->day));
  ds1302_write_to_addr(DS1302_ADDR_HOUR, to_bcd(datetime->hour));
  ds1302_write_to_addr(DS1302_ADDR_MINUTE, to_bcd(datetime->minute));
  ds1302_write_to_addr(DS1302_ADDR_SECOND, to_bcd(datetime->second));
  ds1302_write_to_addr(0x8E, 0x10);
}

/**
 * 读取ds1302到datetime
 * @param datetime [description]
 */
void ds1302_get_datetime(ds1302_Datetime *datetime)
{
  datetime->year   = from_bcd(ds1302_read_from_addr(DS1302_ADDR_YEAR | 0x01));
  datetime->week   = from_bcd(ds1302_read_from_addr(DS1302_ADDR_WEEK | 0x01));
  datetime->month  = from_bcd(ds1302_read_from_addr(DS1302_ADDR_MONTH | 0x01));
  datetime->day    = from_bcd(ds1302_read_from_addr(DS1302_ADDR_DAY | 0x01));
  datetime->hour   = from_bcd(ds1302_read_from_addr(DS1302_ADDR_HOUR | 0x01));
  datetime->minute = from_bcd(ds1302_read_from_addr(DS1302_ADDR_MINUTE | 0x01));
  datetime->second = from_bcd(ds1302_read_from_addr(DS1302_ADDR_SECOND | 0x01));
}