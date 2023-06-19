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
#include <memory>

/*
*U8G2APP说明:
*   * 界面的切换依靠 process_event 处理,界面切换中将调用相关动画函数。注意：init_event、idle_running_event由将外部调用,不能由U8G2APP调用。
*   * 当U8G2APP被初始化后,将处于IDLE(界面)状态。
*IDLE界面说明:
*   IDLE界面主要包括以下部分:
*       *backgroud(背景):一些背景边框。
*       *statusbar(状态栏)：对于单色屏幕而言,状态栏一般在顶部。
*       *main:主显示区域。
*       *menu（菜单）：对于单色屏幕而言，若有单独的菜单,单独的菜单一般在底部。
*   注意事项：IDLE界面绘制时,不要调用对整个屏幕有影响的函数(如clearBuffer等),绘制顺序background -> statusbar -> main -> menu,后绘制的可覆盖先绘制的图形。
*
*
*
*/

class u8g2app;

/** \brief 开机动画,默认具有weak属性,需要修改时请重写此函数。
 *
 * \param app u8g2app& 应用
 * \param display U8G2& 显示屏
 *
 */
void u8g2app_booting_animation(u8g2app &app,U8G2 &display);


/** \brief 屏幕空闲,通常用于显示数据后台处理,默认具有weak属性,需要修改时请重写此函数。
 *
 * \param app u8g2app& 应用
 * \param display U8G2& 显示屏
 * \param tick uint32_t 节拍（通常是时钟节拍）
 *
 */
void u8g2app_idle_running(u8g2app &app,U8G2 &display,uint32_t tick);

/** \brief 屏幕空闲背景,默认具有weak属性,需要修改时请重写此函数。
 *
 * \param app u8g2app& 应用
 * \param display U8G2& 显示屏
 * \return bool 是否绘制成功
 *
 */
bool u8g2app_idle_backgroud(u8g2app &app,U8G2 &display);

/** \brief 屏幕空闲状态栏,默认具有weak属性,需要修改时请重写此函数。
 *
 * \param app u8g2app& 应用
 * \param display U8G2& 显示屏
 * \return bool 是否绘制成功
 *
 */
bool u8g2app_idle_statusbar(u8g2app &app,U8G2 &display);

/** \brief 屏幕空闲主显示区域,默认具有weak属性,需要修改时请重写此函数。
 *
 * \param app u8g2app& 应用
 * \param display U8G2& 显示屏
 * \return bool 是否绘制成功
 *
 */
bool u8g2app_idle_main(u8g2app &app,U8G2 &display);

/** \brief 屏幕空闲菜单,默认具有weak属性,需要修改时请重写此函数。
 *
 * \param app u8g2app& 应用
 * \param display U8G2& 显示屏
 * \return bool 是否绘制成功
 *
 */
bool u8g2app_idle_menu(u8g2app &app,U8G2 &display);


/** \brief 屏幕空闲切换动画,默认具有weak属性,需要修改时请重写此函数。
 *
 * \param app u8g2app& 应用
 * \param display U8G2& 显示屏
 * \param new_context std::shared_ptr<void *> 新上下文
 * \param old_context std::shared_ptr<void *> 旧上下文
 *
 */
void u8g2app_idle_change_animation(u8g2app &app,U8G2 &display,std::shared_ptr<void *> new_context,std::shared_ptr<void *> old_context);


class u8g2app:public fsmlite::fsm<u8g2app>
{
    friend class fsmlite::fsm<u8g2app>;
    friend void u8g2app_booting_animation(u8g2app &app,U8G2 &display);
    friend void u8g2app_idle_running(u8g2app &app,U8G2 &display,uint32_t tick);
    friend bool u8g2app_idle_backgroud(u8g2app &app,U8G2 &display);
    friend bool u8g2app_idle_statusbar(u8g2app &app,U8G2 &display);
    friend bool u8g2app_idle_main(u8g2app &app,U8G2 &display);
    friend bool u8g2app_idle_menu(u8g2app &app,U8G2 &display);
    friend void u8g2app_idle_change_animation(u8g2app &app,U8G2 &display,std::shared_ptr<void *> new_context,std::shared_ptr<void *> old_context);
    U8G2 * display;
    std::queue<std::function<void()>> event_queue;
    void event_queue_add(std::function<void()> _cb)
    {
        event_queue.push(_cb);
    }

    //idle界面context
    std::shared_ptr<void *> idle_context;

public:
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
            //空闲屏幕绘制

            bool isok=true;

            if(isok)
            {
                display->clearBuffer();
            }

            if(isok)
            {
                isok=u8g2app_idle_backgroud(*this,*display);
            }

            if(isok)
            {
                isok=u8g2app_idle_statusbar(*this,*display);
            }

            if(isok)
            {
                isok=u8g2app_idle_main(*this,*display);
            }

            if(isok)
            {
                isok=u8g2app_idle_menu(*this,*display);
            }

            if(isok)
            {
                display->sendBuffer();
            }

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

    std::shared_ptr<void *> GetIdleContext()
    {
        return idle_context;
    }

    enum states
    {
        U8G2APP_INIT,
        U8G2APP_IDLE_RUNNING
    };
    //初始化事件
    using init_event=U8G2 *;
    //空闲运行事件
    using idle_running_event=uint32_t;
    //空闲界面改变
    using idle_change_event=std::shared_ptr<void *>;
private:
    using m = u8g2app;


    //状态机转移函数

    //初始化状态转移函数
    void  init(const init_event & event)
    {
        //注意:传入的event不可为空
        display=event;
        //调用开机动画
        event_queue_add([this]()
        {
            u8g2app_booting_animation(*this,*(this->display));
        });
    }

    //空闲运行状态转移函数
    void idle_running(const idle_running_event & event)
    {
        event_queue_add([this,event]()
        {
            u8g2app_idle_running(*this,*(this->display),(uint32_t)event);
        });
    }

    void idle_change(const idle_change_event & event)
    {
        event_queue_add([this,event]()
        {
            u8g2app_idle_change_animation(*this,*(this->display),event,this->idle_context);
            this->idle_context=event;
        });

    }

    //状态转移监视函数,返回true执行状态转移
    bool  guard_init(const init_event & event) const
    {
        return event!=NULL;
    }

    //状态转移表
    using transition_table = table<
                             mem_fn_row< U8G2APP_INIT, init_event,U8G2APP_IDLE_RUNNING, &m::init,&m::guard_init>,
                             mem_fn_row< U8G2APP_IDLE_RUNNING, idle_running_event,U8G2APP_IDLE_RUNNING, &m::idle_running>,
                             mem_fn_row< U8G2APP_IDLE_RUNNING, idle_change_event,U8G2APP_IDLE_RUNNING, &m::idle_change>
                             >;

};
#endif // __cplusplus

#endif // U8G2APP_H
