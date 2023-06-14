#include "gui_u8g2.h"
#include "rtconfig.h"
#include "rtthread.h"
#include "rtdevice.h"
#ifndef U8G2_USE_CPP

void gui_u8g2_init()
{

}
void gui_u8g2_loop()
{

}

#else
#ifdef U8G2_USE_HW_I2C
#include "rtdevice.h"
#include "U8g2lib.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
static U8G2_SSD1306_128X64_NONAME_F_HW_I2C *display;
void gui_u8g2_init()
{
    {
        struct rt_i2c_bus_device *i2cbus=rt_i2c_bus_device_find(U8G2_I2C_DEVICE_NAME);
        if(i2cbus==NULL)
        {
            return;
        }

    }

    //初始化U8G2
    display=new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0);
    if(display!=NULL)
    {
        //基本初始化
        display->begin();
        display->enableUTF8Print();
        display->setFont(u8g2_font_unifont_t_chinese2);

        //显示Booting字样
        display->clearBuffer();
        display->setCursor(0,32);
        display->print("Booting!");
        display->sendBuffer();
    }
}
void gui_u8g2_loop()
{

}
#endif // U8G2_USE_HW_I2C
#endif

