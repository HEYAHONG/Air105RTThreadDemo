#include "u8g2app.h"

#ifndef U8G2APP_WEAK
#define U8G2APP_WEAK __attribute__((weak))
#endif // U8G2APP_WEAK

U8G2APP_WEAK void  u8g2app_booting_animation(u8g2app &app,U8G2 &display)
{
    uint8_t w=display.getWidth();
    uint8_t h=display.getHeight();

    //显示启动中字样
    display.setFont(u8g2_font_wqy12_t_gb2312);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);
    display.setFont(u8g2_font_wqy13_t_gb2312);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);
    display.setFont(u8g2_font_wqy14_t_gb2312);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);
    display.setFont(u8g2_font_wqy15_t_gb2312);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);
    display.setFont(u8g2_font_wqy16_t_gb2312);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);

}

U8G2APP_WEAK void u8g2app_idle_running(u8g2app &app,U8G2 &display,uint32_t tick)
{

}
