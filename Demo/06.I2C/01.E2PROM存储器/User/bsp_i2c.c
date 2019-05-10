#include "bsp_i2c.h"
#include "bsp_delay.h"

sbit SCL = P2^6;
sbit SDA = P2^5;

/* 初始化 I2C 总线 */
void i2c_init(void)
{
    SCL = P2^7;
    SDA = P2^6;
}

/**
 * @brief 控制 I2C 的速度
 * @param i2c_bus_x 指定操作的 I2C 对象
 */
void i2c_delay(void)
{
    delay_us(200);
}

/**
 * @brief 产生 I2C 起始信号
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @note 在 scl 为高电平的情况下, sda 产生一个下降沿表示起始
 */
void i2c_start(void)
{
    /* 这里需要注意顺序, sda 先拉高, scl 再拉高 */
    /* 因为 scl 为高电平的情况下, sda 的边沿动作是有意义的, 因此 sda 要先拉高 */
    SDA = 1;
    SCL = 1;
    i2c_delay();

    /* 在 scl 为高电平的情况下, sda 产生一个下降沿表示起始*/
    SDA = 0;
    i2c_delay();

    /* 这里其实就是开始驱动传输的时钟了 */
    SCL = 0;
}

/**
 * @brief 产生 I2C 停止信号
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @note 在 scl 为高电平的情况下, sda 产生一个上升沿表示停止
 */
void i2c_stop(void)
{
    /* 这里需要注意顺序, scl 先拉低, sda 再拉低 */
    /* 因为 scl 为高电平的情况下, sda 的边沿动作是有意义的, 因此 scl 要先拉低 */
    SCL = 0;
    SDA = 0;
    i2c_delay();

    SCL = 1;
    i2c_delay();

    SDA = 1;
    i2c_delay();
}

/**
 * @brief 等待应答信号到来
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @return u8 0,接收应答成功; 1,接收应答失败;
 * @note 主机发送完一帧数据之后, 需要等待从机给出响应才会继续发出下一帧数据
 * @note 这时候主机需要放出 sda 的使用权, 由从机负责拉低 sda (响应)
 * @note 主机在下一个 scl 的上升沿(或说高电平)检测 sda 是否为低电平, 低电平则表示有应答
 */
int i2c_wait_response(void)
{
    /* 定义一个变量, 作为超时的标记 */
    int ucErrTime = 0;

    /* 先默认的把 sda 设置为高电平 */
    SDA = 1;
    i2c_delay();

    /* 下一个 scl 时钟到了 */
    SCL = 1;
    i2c_delay();

    /* 是时候去读取 sda 看看有没有响应了 */
    /* 在没有超时之前只要读到了应答就可以自动跳出 while */
    while(SDA)
    {
        ucErrTime++;

        /* 超时了, 既然从机在规定的时间不应答*/
        if(ucErrTime > 255)
        {
            /* 主机就认为从机没有正确的接收, 就此作罢 */
            i2c_stop();

            /* 函数返回接收应答失败 */
            return -1;
        }
    }

    /* 到这里, 读取应答信号的 scl 结束了 */
    SCL = 0;

    /* 能执行到这里, 说明读取到了应答的 */
    return 0;
}

/**
 * @brief HOST 产生应答
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @note 此函数是在 HOST(STM32) 接收数据时才能使用
 * @note 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
void i2c_response(void)
{
    SCL = 0;

    /* 在从机发送完本帧最后一个位的低电平期间, 把 sda 拉低 */
    /* 千万不能在 scl 高电平期间拉低, 那就变成起始信号了 */
    SDA = 0;
    i2c_delay();

    /* 这里就是从机读取应答信号的一个 scl 时钟周期了, sda 不能动 */
    SCL = 1;
    i2c_delay();
    SCL = 0;
}

/**
 * @brief HOST 不产生应答
 * @param i2c_bus_x  指定操作的 I2C 对象
 * @note 此函数是在 HOST(STM32) 接收数据时才能使用
 * @note 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
void i2c_no_response(void)
{
    SCL = 0;

    /* 既然拉低表示应答, 那我拉高不就行了 */
    SDA = 1;
    i2c_delay();

    /* 下一个时钟一检测, 就发现, 没有应答 */
    SCL = 1;
    i2c_delay();
    SCL = 0;
}

/**
 * @brief I2C 发送一个字节
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @param dat 发送的数据
 */
void i2c_send_byte(u8 dat)
{
    u8 t;

    /* 所有的数据的输出到 sda 线上都是在 scl 的低电平期间 */
    SCL = 0;

    /* 依次发送8个数据值 */
    for(t = 0; t < 8; t++)
    {
        /* 写入数据的最高位 */
        SDA = (dat& 0x80) >> 7;

        /* 发送完了最高位, 数据左移一个, 次高位变成了新的最高位 */
        dat <<= 1;
        i2c_delay();

        /* 在 scl 的上升沿(或者高电平期间), 数据被从机接收读取 */
        SCL = 1;
        i2c_delay();
        SCL = 0;
        i2c_delay();
    }
    /* 这函数结束的时候是不是 scl = 0 */
    /* 一般这里接下来就会是 WaitResponse 操作了 */
    /* WaitResponse 的时候就是直接从 scl = 0 开始的, 这样就不会多出来一个 scl 的脉冲 */
}

/**
 * @brief I2C 读一个字节
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @note 根据参数决定要不要应答, 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
u8 i2c_receive_byte(void)
{
    u8 i, dat = 0;

    for(i = 0; i < 8; i++)
    {
        SCL = 0;
        i2c_delay();
        SCL = 1;
        dat <<= 1;
        if(SDA)
            dat++;
        i2c_delay();
    }

    return dat;
}
