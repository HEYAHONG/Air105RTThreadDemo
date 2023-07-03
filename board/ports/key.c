#include "key.h"
#include "rtconfig.h"
#include "rtthread.h"
#include "board.h"
#include "stdbool.h"
#include <board.h>
#include "drv_gpio.h"

static struct
{
    key_event_callback_t cb;
    void *usr;
} handler= {NULL,NULL};

int key_set_event_handler(key_event_callback_t cb,void* usr)
{
    handler.usr=usr;
    handler.cb=cb;
    return RT_EOK;
}

static void key_emit_event(key_index_t i,key_event_type_t type)
{
    if(handler.cb!=NULL)
    {
        handler.cb((key_index_t)i,type,handler.usr);
    }
}


#ifdef BSP_USING_KEY

#define KEY_PIN GPIOA_10

static void key0_init()
{
    rt_pin_mode(KEY_PIN,PIN_MODE_INPUT_PULLUP);
}

static bool key0_ispress()
{
    return rt_pin_read(KEY_PIN)==PIN_LOW;
}


static struct
{
    void (*init)();
    bool (*ispress)();
    uint8_t press_cnt;
    struct
    {
        uint8_t ispress;//0=未初始化,1=按下,2=释放
        rt_tick_t tick;
    } history[10];
    uint8_t current_history_index;
    uint8_t last_history_index;

} key[KEY_INDEX_MAX] =
{
    {
        key0_init,
        key0_ispress,
        0,
        {0},
        0,
        0
    }
};

static void key_timer_timeout( void *para)
{
    for(size_t i=0; i<sizeof(key)/sizeof(key[0]); i++)
    {
        if(key[i].ispress!=NULL)
        {
            if(key[i].ispress())
            {
                key[i].press_cnt++;
                if(key[i].press_cnt > 100)
                {
                    key[i].press_cnt--;
                }
            }
            else
            {
                key[i].press_cnt=0;
            }

            if(key[i].press_cnt > 3)
            {
                if(key[i].press_cnt == 4)
                {
                    //记录按下事件
                    key[i].current_history_index++;
                    if(key[i].current_history_index >= sizeof(key[i].history)/sizeof(key[i].history[0]))
                    {
                        key[i].current_history_index=0;
                    }
                    key[i].history[key[i].current_history_index].ispress = 1;
                    key[i].history[key[i].current_history_index].tick=rt_tick_get();

                    //按下
                    key_emit_event((key_index_t)i,KEY_EVENT_TYPE_PRESS);


                }

                //处于按下状态
                key_emit_event((key_index_t)i,KEY_EVENT_TYPE_ON_PRESS);

                if(key[i].last_history_index != key[i].current_history_index)
                {
                    if(key[i].history[key[i].current_history_index].ispress == 1)
                    {
                        if( rt_tick_get()-key[i].history[key[i].current_history_index].tick > (RT_TICK_PER_SECOND*1000/1000))
                        {
                            //长按超过1000ms
                            key[i].last_history_index=key[i].current_history_index;
                            //长按
                            key_emit_event((key_index_t)i,KEY_EVENT_TYPE_LONG_PRESS);

                        }
                    }
                }
            }
            else
            {
                if(key[i].history[key[i].current_history_index].ispress == 1)
                {
                    //记录释放事件
                    key[i].current_history_index++;
                    if(key[i].current_history_index >= sizeof(key[i].history)/sizeof(key[i].history[0]))
                    {
                        key[i].current_history_index=0;
                    }
                    key[i].history[key[i].current_history_index].ispress = 2;
                    key[i].history[key[i].current_history_index].tick=rt_tick_get();

                    //松开
                    key_emit_event((key_index_t)i,KEY_EVENT_TYPE_RELEASE);
                }

                //处于释放状态
                key_emit_event((key_index_t)i,KEY_EVENT_TYPE_ON_RELEASE);


                if(key[i].last_history_index != key[i].current_history_index)
                {
                    if(key[i].history[key[i].current_history_index].ispress == 2)
                    {
                        if( rt_tick_get()-key[i].history[key[i].current_history_index].tick > (RT_TICK_PER_SECOND*200/1000))
                        {
                            //释放超过200ms
                            uint8_t current_history_index=key[i].current_history_index;
                            uint8_t last_history_index=key[i].last_history_index;
                            if(current_history_index < last_history_index)
                            {
                                current_history_index+=sizeof(key[i].history)/sizeof(key[i].history[0]);
                            }

                            if(current_history_index-last_history_index == 2)
                            {
                                //单击
                                key_emit_event((key_index_t)i,KEY_EVENT_TYPE_CLICK);

                            }

                            if(current_history_index-last_history_index >= 4)
                            {
                                //双击(更多连击也算双击)
                                key_emit_event((key_index_t)i,KEY_EVENT_TYPE_DOUBLE_CLICK);
                            }


                            key[i].last_history_index=key[i].current_history_index;
                        }
                    }
                }


            }

        }
    }
}

static int key_init()
{

    for(size_t i=0; i<sizeof(key)/sizeof(key[0]); i++)
    {
        if(key[i].init!=NULL)
        {
            key[i].init();
        }
    }

    static rt_timer_t timer=NULL;
    /*
        每5ms执行一次
    */
    timer=rt_timer_create("key",key_timer_timeout,&timer,RT_TICK_PER_SECOND*5/1000,RT_TIMER_FLAG_SOFT_TIMER|RT_TIMER_FLAG_PERIODIC);
    if(timer!=NULL)
    {
        rt_timer_start(timer);
    }

    return RT_EOK;
}

INIT_ENV_EXPORT(key_init);

#endif // BSP_USING_KEY
