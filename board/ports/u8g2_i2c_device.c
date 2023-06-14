#include "rtconfig.h"
#include <rtdevice.h>
#include <rtthread.h>
int u8g2_i2c_device_init(void)
{
    if(rt_strcmp(I2C_BUS_NAME,U8G2_I2C_DEVICE_NAME)!=0)
    {
        rt_kprintf("[u8g2] u8g2 kconfig error!(U8G2_I2C_DEVICE_NAME != I2C_BUS_NAME).");
    }
}
INIT_ENV_EXPORT(u8g2_i2c_device_init);
