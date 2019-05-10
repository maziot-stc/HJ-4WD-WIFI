#include "bsp_at24c256.h"
#include "bsp_i2c.h"
#include "bsp_uart.h"

sbit AT24C256_WP = P2^7;

/**
 * @brief 设置即将操作的地址
 * @param address 指定要写入的地址
 * @return int 返回程序执行的状态
 */
int at24c256_set_current_address(u8 address)
{
    u8 address_high = (u8)((address >> 8) & 0x00ff);
    u8 address_low = (u8)(address & 0x00ff);

    AT24C256_WP = WP_ENABLE;

    i2c_start();

    i2c_send_byte(AT24C256_WRITE);
    if(i2c_wait_response())
    {
        printf("device address mapping fail\r\n");
        return -1;
    }

    i2c_send_byte(address_high);
    if(i2c_wait_response())
    {
        printf("write address fail\r\n");
        return -1;
    }
    i2c_send_byte(address_low);
    if(i2c_wait_response())
    {
        printf("write address fail\r\n");
        return -1;
    }

    i2c_stop();
    AT24C256_WP = WP_ENABLE;
    return 0;
}

/**
 * @brief 在指定的地址写一个字节
 * @param address 指定要写入的地址
 * @param buffer 要写入数据的数据缓存区
 * @return int 返回程序执行的状态
 */
int at24c256_byte_write(u16 address, u8* buffer)
{
    u8 address_high = (u8)((address >> 8) & 0x00ff);
    u8 address_low = (u8)(address & 0x00ff);
    AT24C256_WP = WP_DISABLE;

    i2c_start();

    i2c_send_byte(AT24C256_WRITE);
    if(i2c_wait_response())
    {
        printf("device address mapping fail\r\n");
        return -1;
    }

    i2c_send_byte(address_high);
    if(i2c_wait_response())
    {
        printf("write address fail\r\n");
        return -1;
    }
    i2c_send_byte(address_low);
    if(i2c_wait_response())
    {
        printf("write address fail\r\n");
        return -1;
    }

    i2c_send_byte(*buffer);
    if(i2c_wait_response())
    {
        printf("write data fail\r\n");
        i2c_stop();
        return -1;
    }

    i2c_stop();
    AT24C256_WP = WP_ENABLE;
    return 0;
}

/**
 * @brief 在指定的地址连续写入字节
 * @param address 指定要写入的首地址
 * @param buffer 要写入数据的数据缓存区
 * @param length 指定要写入的字节个数
 * @return int 返回程序执行的状态
 * @note 此函数为页写入函数, 对于 at24c256 一页为 64 个字节
 */
int at24c256_page_write(u16 address, u8* buffer, u8 length)
{
    u8 address_high = (u8)((address >> 8) & 0x00ff);
    u8 address_low = (u8)(address & 0x00ff);
    u8 i;
    AT24C256_WP = WP_DISABLE;

    i2c_start();

    if((length <= 0) || (length > 64))
    {
        printf("length parameter values are not specification\r\n");
        return -1;
    }

    i2c_send_byte(AT24C256_WRITE);
    if(i2c_wait_response())
    {
        printf("device address mapping fail\r\n");
        return -1;
    }

    i2c_send_byte(address_high);
    if(i2c_wait_response())
    {
        printf("write address fail\r\n");
        return -1;
    }
    i2c_send_byte(address_low);
    if(i2c_wait_response())
    {
        printf("write address fail\r\n");
        return -1;
    }

    for(i = 0; i < length; i++)
    {
        i2c_send_byte(buffer[i]);
        if(i2c_wait_response())
        {
            printf("write data error at %d\r\n", i);
            return -1;
        }
    }

    i2c_stop();
    AT24C256_WP = WP_ENABLE;
    return 0;
}

/**
 * @brief 直接读取当前地址的内容
 * @param buffer 保存读取到的数据的缓存区
 * @return int 返回程序执行的状态
 */
int at24c256_current_address_read(u8* buffer)
{
    AT24C256_WP = WP_ENABLE;
    i2c_start();

    i2c_send_byte(AT24C256_READ);
    if(i2c_wait_response())
    {
        printf("device address mapping fail\r\n");
        return -1;
    }

    buffer[0] = i2c_receive_byte();

    i2c_no_response();

    i2c_stop();
    AT24C256_WP = WP_ENABLE;
    return 0;
}

/**
 * @brief 在指定的地址读取一片连续的字节
 * @param address 指定要读取的首地址
 * @param buffer 保存读取到的数据的缓存区
 * @return int 返回程序执行的状态
 */
int at24c256_random_read(u16 address, u8* buffer)
{
    u8 address_high = (u8)((address >> 8) & 0x00ff);
    u8 address_low = (u8)(address & 0x00ff);
    AT24C256_WP = WP_ENABLE;

    i2c_start();

    i2c_send_byte(AT24C256_WRITE);
    if(i2c_wait_response())
    {
        printf("device address mapping fail\r\n");
        return -1;
    }

    i2c_send_byte(address_high);
    if(i2c_wait_response())
    {
        printf("write address fail\r\n");
        return -1;
    }
    i2c_send_byte(address_low);
    if(i2c_wait_response())
    {
        printf("write address fail\r\n");
        return -1;
    }
    AT24C256_WP = WP_ENABLE;
    return at24c256_current_address_read(buffer);
}
