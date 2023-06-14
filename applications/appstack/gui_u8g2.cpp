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
#ifdef BSP_USING_U8G2_HWI2C
#include "rtdevice.h"
#include "U8g2lib.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

#ifdef BSP_USING_U8G2_DISPLAY_CONTROLER_SSD1306
#define DISPLAY_CONTROLER SSD1306
#endif // BSP_USING_U8G2_DISPLAY_CONTROLER_SSD1306

#ifdef BSP_USING_U8G2_DISPLAY_CONTROLER_SH1106
#define DISPLAY_CONTROLER SH1106
#endif // BSP_USING_U8G2_DISPLAY_CONTROLER_SH1106

#ifdef BSP_USING_U8G2_DISPLAY_RESOLUTION_128X64
#define DISPLAY_RESOLUTION 128X64
#endif // BSP_USING_U8G2_DISPLAY_RESOLUTION_128X64

#ifndef DISPLAY_CONTROLER
#define DISPLAY_CONTROLER SSD1306
#endif // DISPLAY_CONTROLER

#ifndef DISPLAY_RESOLUTION
#define DISPLAY_RESOLUTION 128X64
#endif // DISPLAY_RESOLUTION

#define DISPLAY_DEFINE1(CONTROLER,RESOLUTION) U8G2_##CONTROLER##_##RESOLUTION##_NONAME_F_HW_I2C
#define DISPLAY_DEFINE2(CONTROLER,RESOLUTION) DISPLAY_DEFINE1(CONTROLER,RESOLUTION)
#define DISPLAY DISPLAY_DEFINE2(DISPLAY_CONTROLER,DISPLAY_RESOLUTION)

static DISPLAY *display=NULL;
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
    display=new DISPLAY(U8G2_R0);
    if(display!=NULL)
    {
        //基本初始化
        display->begin();
        display->enableUTF8Print();
        display->setFont(u8g2_font_wqy16_t_gb2312);

        //显示Booting字样
        display->clearBuffer();
        display->setCursor(0,32);
        display->print("启动中...");
        display->sendBuffer();
    }
}
void gui_u8g2_loop()
{

}
#endif // BSP_USING_U8G2_HWI2C
#endif

