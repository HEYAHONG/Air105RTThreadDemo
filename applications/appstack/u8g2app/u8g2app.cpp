#include "u8g2app.h"
#include "chrono"
#ifndef U8G2APP_WEAK
#define U8G2APP_WEAK __attribute__((weak))
#endif // U8G2APP_WEAK

#ifdef U8G2_HAS_FONT_USER
#include "u8g2_font_user.h"
#endif // U8G2_HAS_FONT_USER

#ifdef U8G2_HAS_FONT_USER
#define FONT_WQY12_T u8g2_font_wqy12_t_user
#else
#define FONT_WQY12_T u8g2_font_wqy12_t_gb2312
#endif // U8G2_HAS_FONT_USER

#ifdef U8G2_HAS_FONT_USER
#define FONT_WQY13_T u8g2_font_wqy13_t_user
#else
#define FONT_WQY13_T u8g2_font_wqy13_t_gb2312
#endif // U8G2_HAS_FONT_USER

#ifdef U8G2_HAS_FONT_USER
#define FONT_WQY14_T u8g2_font_wqy14_t_user
#else
#define FONT_WQY14_T u8g2_font_wqy14_t_gb2312
#endif // U8G2_HAS_FONT_USER

#ifdef U8G2_HAS_FONT_USER
#define FONT_WQY15_T u8g2_font_wqy15_t_user
#else
#define FONT_WQY15_T u8g2_font_wqy15_t_gb2312
#endif // U8G2_HAS_FONT_USER

#ifdef U8G2_HAS_FONT_USER
#define FONT_WQY16_T u8g2_font_wqy16_t_user
#else
#define FONT_WQY16_T u8g2_font_wqy16_t_gb2312
#endif // U8G2_HAS_FONT_USER

U8G2APP_WEAK void  u8g2app_booting_animation(u8g2app &app,U8G2 &display)
{
    uint8_t w=display.getWidth();
    uint8_t h=display.getHeight();

    //显示启动中字样
    display.setFont(FONT_WQY12_T);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);
    display.setFont(FONT_WQY13_T);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);
    display.setFont(FONT_WQY14_T);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);
    display.setFont(FONT_WQY15_T);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);
    display.setFont(FONT_WQY16_T);
    display.clearBuffer();
    display.drawFrame(0,0,w,h);
    display.setCursor(5,h/2);
    display.print("启动中...");
    display.sendBuffer();
    msleep(400);

}

U8G2APP_WEAK void u8g2app_idle_running(u8g2app &app,U8G2 &display,uint32_t tick)
{
    //界面上下文
    std::shared_ptr<void> context=app.GetIdleContext();
}

U8G2APP_WEAK bool u8g2app_idle_backgroud(u8g2app &app,U8G2 &display)
{
    //界面上下文
    std::shared_ptr<void> context=app.GetIdleContext();

    uint8_t w=display.getWidth();
    uint8_t h=display.getHeight();
    display.drawFrame(0,0,w,h);
    return true;
}

U8G2APP_WEAK bool u8g2app_idle_statusbar(u8g2app &app,U8G2 &display)
{
    //界面上下文
    std::shared_ptr<void> context=app.GetIdleContext();

    uint8_t w=display.getWidth();
    uint8_t h=display.getHeight();
    display.setFont(u8g2_font_wqy12_t_gb2312);
    uint8_t font_width=12;
    uint8_t max_char=w/font_width;
    display.drawFrame(0,0,w,h/5<12?12:h/5+3);
    display.setCursor(font_width*(max_char-4)/2,font_width+1);
    display.print("空闲界面");
    return true;
}

U8G2APP_WEAK bool u8g2app_idle_main(u8g2app &app,U8G2 &display)
{
    //界面上下文
    std::shared_ptr<void> context=app.GetIdleContext();

    uint8_t w=display.getWidth();
    uint8_t h=display.getHeight();
    uint8_t start_x=0;
    uint8_t start_y=h/5<12?12:h/5+3;
    display.setFont(u8g2_font_wqy12_t_gb2312);
    uint8_t font_width=12;
    uint8_t max_char=w/font_width;

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time_t=std::chrono::system_clock::to_time_t(now);
    char buff[50]= {0};
    sprintf(buff,"%lu",(unsigned long)now_time_t);
    display.setCursor(font_width*(max_char-strlen(buff)/2)/2,start_y+font_width*2);
    display.print(buff);
    return true;
}

U8G2APP_WEAK bool u8g2app_idle_menu(u8g2app &app,U8G2 &display)
{
    //界面上下文
    std::shared_ptr<void> context=app.GetIdleContext();

    uint8_t w=display.getWidth();
    uint8_t h=display.getHeight();
    uint8_t start_x=0;
    uint8_t start_y=h-(h/5<12?12:h/5+3);

    return true;
}

U8G2APP_WEAK void u8g2app_idle_change_animation(u8g2app &app,U8G2 &display,std::shared_ptr<void> new_context,std::shared_ptr<void> old_context)
{

}
