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

#ifndef DISPLAY_NAME
#define DISPLAY_NAME NONAME
#endif // DISPLAY_NAME

#define DISPLAY_DEFINE1(CONTROLER,RESOLUTION,NAME) U8G2_##CONTROLER##_##RESOLUTION##_##NAME##_F_HW_I2C
#define DISPLAY_DEFINE2(CONTROLER,RESOLUTION,NAME) DISPLAY_DEFINE1(CONTROLER,RESOLUTION,NAME)
#define DISPLAY DISPLAY_DEFINE2(DISPLAY_CONTROLER,DISPLAY_RESOLUTION,DISPLAY_NAME)

static DISPLAY *display=NULL;
static void gui_thread_entry(void *parameter)
{
    static DISPLAY *display=(DISPLAY *)parameter;
    if(display==NULL)
    {
        return;
    }

    //显示启动中字样
    display->setFont(u8g2_font_wqy12_t_gb2312);
    display->clearBuffer();
    display->setCursor(0,32);
    display->print("启动中...");
    display->sendBuffer();
    rt_thread_mdelay(400);

    display->setFont(u8g2_font_wqy13_t_gb2312);
    display->clearBuffer();
    display->setCursor(0,32);
    display->print("启动中...");
    display->sendBuffer();
    rt_thread_mdelay(400);

    display->setFont(u8g2_font_wqy14_t_gb2312);
    display->clearBuffer();
    display->setCursor(0,32);
    display->print("启动中...");
    display->sendBuffer();
    rt_thread_mdelay(400);

    display->setFont(u8g2_font_wqy15_t_gb2312);
    display->clearBuffer();
    display->setCursor(0,32);
    display->print("启动中...");
    display->sendBuffer();
    rt_thread_mdelay(400);

    display->setFont(u8g2_font_wqy16_t_gb2312);
    display->clearBuffer();
    display->setCursor(0,32);
    display->print("启动中...");
    display->sendBuffer();
    rt_thread_mdelay(400);


    while(true)
    {
        rt_thread_mdelay(25);
    }
}


void gui_u8g2_init()
{
    {
        //检查I2C总线设备是否存在
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

        rt_thread_t tid=rt_thread_create("gui",gui_thread_entry,display, RT_MAIN_THREAD_STACK_SIZE, RT_MAIN_THREAD_PRIORITY, 20);
        if(tid!=NULL)
        {
            rt_thread_startup(tid);
        }

    }
}
void gui_u8g2_loop()
{

}
#endif // BSP_USING_U8G2_HWI2C
#endif

