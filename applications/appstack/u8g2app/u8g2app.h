#ifndef U8G2APP_H
#define U8G2APP_H

#include "U8g2lib.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "fsm.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef msleep
void msleep(unsigned int msecs);
#endif // msleep

#ifdef __cplusplus
}
#endif // __cplusplus


#ifdef __cplusplus
#include <queue>
#include <functional>


class u8g2app:public fsmlite::fsm<u8g2app>
{
    friend class fsmlite::fsm<u8g2app>;
    U8G2 * display;
    std::queue<std::function<void()>> event_queue;
    void event_queue_add(std::function<void()> _cb)
    {
        event_queue.push(_cb);
    }
public:
    enum states
    {
        U8G2APP_INIT,
        U8G2APP_IDLE_RUNNING
    };
    using init_event=U8G2 *;
    using idle_running_event=uint32_t;
    u8g2app():fsm(U8G2APP_INIT),display(NULL)
    {

    }

    /*
     * 状态机运行循环,将被外部调用多次.可在此函数中调用process_event进行状态转移,注意:不可在状态转移函数中调用process_event
     * 如需在转移函数中调用process_event,需要使用event_queue_add将对应的lambda函数添加至队列
     */
    void running()
    {

        //根据当前状态进行处理
        switch(current_state())
        {
        case U8G2APP_INIT:
        {

        }
        break;
        case U8G2APP_IDLE_RUNNING:
        {

        }
        break;
        default:
            break;
        }

        //处理事件队列中的事件
        {
            while(event_queue.size()>0)
            {
                std::function<void()> cb=event_queue.front();
                event_queue.pop();
                if(cb!=NULL)
                {
                    try
                    {
                        cb();
                    }
                    catch(...)
                    {

                    }
                }
            }
        }

    }
private:
    using m = u8g2app;


    //状态机转移函数

    //初始化状态转移函数
    void  init(const init_event & event)
    {
        //注意:传入的event不可为空
        display=event;

        uint8_t w=display->getWidth();
        uint8_t h=display->getHeight();



        //显示启动中字样
        display->setFont(u8g2_font_wqy12_t_gb2312);
        display->clearBuffer();
        display->drawFrame(0,0,w,h);
        display->setCursor(5,h/2);
        display->print("启动中...");
        display->sendBuffer();
        msleep(400);

        display->setFont(u8g2_font_wqy13_t_gb2312);
        display->clearBuffer();
        display->drawFrame(0,0,w,h);
        display->setCursor(5,h/2);
        display->print("启动中...");
        display->sendBuffer();
        msleep(400);

        display->setFont(u8g2_font_wqy14_t_gb2312);
        display->clearBuffer();
        display->drawFrame(0,0,w,h);
        display->setCursor(5,h/2);
        display->print("启动中...");
        display->sendBuffer();
        msleep(400);

        display->setFont(u8g2_font_wqy15_t_gb2312);
        display->clearBuffer();
        display->drawFrame(0,0,w,h);
        display->setCursor(5,h/2);
        display->print("启动中...");
        display->sendBuffer();
        msleep(400);

        display->setFont(u8g2_font_wqy16_t_gb2312);
        display->clearBuffer();
        display->drawFrame(0,0,w,h);
        display->setCursor(5,h/2);
        display->print("启动中...");
        display->sendBuffer();

        msleep(400);
    }

    //空闲运行状态转移函数
    void idle_running(const idle_running_event & event)
    {

    }

    //状态转移监视函数,返回true执行状态转移
    bool  guard_init(const init_event & event) const
    {
        return event!=NULL;
    }

    //状态转移表
    using transition_table = table<
                             mem_fn_row< U8G2APP_INIT, init_event,U8G2APP_IDLE_RUNNING, &m::init,&m::guard_init>,
                             mem_fn_row< U8G2APP_IDLE_RUNNING, idle_running_event,U8G2APP_IDLE_RUNNING, &m::idle_running>
                             >;

};
#endif // __cplusplus

#endif // U8G2APP_H
