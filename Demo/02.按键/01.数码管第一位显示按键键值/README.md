
### 问题记录

最开始在编写按键驱动的时候，考虑到 P2 的低4位控制数码管的位选，高4位检测按键是否按下，想要互不影响，因此在控制数码管时，特意只修改低4位的值，不去动高4位。

    #define Segment P0
    #define Digital P2

    u8 SegCode[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
    u8 DigCode[4] = {0xfe, 0xfd, 0xfb, 0xf7};

    int value[4] = {0, 0, 0, 0};

    int main(void)
    {
        while(1)
        {
            /* 或0、与1值不变 */
            Digital = (Digital | 0x0f) & DigCode[0];
            Segment = SegCode[value[0]];
            delay_ms(10);
            key_control(key_scan());
        }
    }

但是实际验证下来，发现第一次按按键是有效的，后面按按键就没有效果了。
排查后发现，原来是我上面的逻辑将按键按下后的 pin 的低电平信息记录下来，并送给 P2 输出低电平。因此按键按下后，按键接的 pin 脚就一直维持低电平，导致按键功能失效。