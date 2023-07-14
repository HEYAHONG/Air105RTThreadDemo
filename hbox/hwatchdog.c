/***************************************************************
 * Name:      hwatchdog.c
 * Purpose:   实现hwatchdog接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/


#include "hwatchdog.h"
typedef struct hwatchdog_watch
{
    struct hwatchdog_watch *next;
    hwatchdog_watch_info_t info;
    bool (*check)(hwatchdog_watch_info_t *info);
    uint32_t timeout_ms;
    uint64_t tick;
} hwatchdog_watch_t;

static struct
{
    //用户参数
    void *usr;
    //获取内存,第一个参数为待获取的内存大小,第二个参数为用户参数。
    void *(*mem_alloc)(size_t,void *);
    //释放内存,第一个参数为待释放的内存指针,第二个参数为用户参数。
    void (*mem_free)(void *,void *);
    //硬件看门狗喂狗
    void (*hw_feed)();
    //系统复位
    void (*sys_reset)();
    //系统获取节拍
    uint64_t (*sys_tick_ms)();
    //检查起始
    hwatchdog_watch_t *watch_start;

} hwatchdog_dog = {0};

static void*default_malloc(size_t nBytes,void *usr)
{
    return malloc(nBytes);
}

static void default_free(void *ptr,void *usr)
{
    return free(ptr);
}


static void check_watchdog_parameter()
{
    if(hwatchdog_dog.mem_alloc==NULL)
    {
        hwatchdog_dog.mem_alloc=default_malloc;
    }

    if(hwatchdog_dog.mem_free==NULL)
    {
        hwatchdog_dog.mem_free=default_free;
    }
}

void hwatchdog_set_memmang(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *))
{
    check_watchdog_parameter();

    hwatchdog_dog.usr=usr;

    if(mem_alloc!=NULL)
    {
        hwatchdog_dog.mem_alloc=mem_alloc;
    }

    if(mem_free!=NULL)
    {
        hwatchdog_dog.mem_free=mem_free;
    }
}

void *hwatchdog_get_usr_ptr()
{
    check_watchdog_parameter();

    return hwatchdog_dog.usr;
}

void hwatchdog_set_hardware_dog_feed(void (*hw_feed)())
{
    hwatchdog_dog.hw_feed=hw_feed;
}

void hwatchdog_setup_software_dog(void (*sys_reset)(),uint64_t (*sys_tick_ms)())
{
    hwatchdog_dog.sys_reset=sys_reset;
    hwatchdog_dog.sys_tick_ms=sys_tick_ms;
}

bool hwatchdog_is_valid()
{
    check_watchdog_parameter();

    if(hwatchdog_dog.hw_feed!=NULL)
    {
        return true;
    }

    if(hwatchdog_dog.sys_reset!=NULL && hwatchdog_dog.sys_tick_ms!=NULL)
    {
        return true;
    }

    return false;
}

void hwatchdog_feed()
{
    check_watchdog_parameter();

    bool is_all_ok=true;

    hwatchdog_watch_t *watch=hwatchdog_dog.watch_start;

    while(watch!=NULL)
    {
        if(watch->check!=NULL)
        {
            if(!watch->check(&watch->info))
            {
                if(hwatchdog_dog.sys_tick_ms!=NULL)
                {
                    if(watch->timeout_ms < (hwatchdog_dog.sys_tick_ms()-watch->tick))
                    {
                        //软件看门狗超时
                        if(hwatchdog_dog.sys_reset!=NULL)
                        {
                            hwatchdog_dog.sys_reset();
                            //理论上不会走到这一步，添加以增加健壮性
                            is_all_ok=false;
                            break;
                        }
                        else
                        {
                            //若无系统复位，需要靠硬件看门狗复位
                            is_all_ok=false;
                            break;
                        }
                    }
                }
                else
                {
                    //检查失败，不喂狗
                    is_all_ok=false;
                    break;
                }
            }
            else
            {
                if(hwatchdog_dog.sys_tick_ms!=NULL)
                {
                    watch->tick=hwatchdog_dog.sys_tick_ms();
                }
            }
        }
        watch=watch->next;
    }

    if(is_all_ok)
    {
        if(hwatchdog_dog.hw_feed!=NULL)
        {
            hwatchdog_dog.hw_feed();
        }
    }
}

void hwatchdog_add_watch(bool (*check)(hwatchdog_watch_info_t *info),uint32_t timeout_ms,hwatchdog_watch_info_t info)
{
    check_watchdog_parameter();

    if(check==NULL)
    {
        //必须添加检查函数
        return;
    }

    hwatchdog_watch_t *watch=(hwatchdog_watch_t *)hwatchdog_dog.mem_alloc(sizeof(hwatchdog_watch_t),hwatchdog_dog.usr);


    if(watch==NULL)
    {
        return;
    }

    memset(watch,0,sizeof(hwatchdog_watch_t));

    watch->check=check;
    watch->info=info;
    watch->next=NULL;
    watch->timeout_ms=timeout_ms;
    watch->tick=0;
    if(hwatchdog_dog.sys_tick_ms!=NULL)
    {
        watch->tick=hwatchdog_dog.sys_tick_ms();
    }

    if(hwatchdog_dog.watch_start==NULL)
    {
        hwatchdog_dog.watch_start=watch;
        return;
    }

    hwatchdog_watch_t *watch_start=hwatchdog_dog.watch_start;

    while(watch_start!=NULL)
    {
        if(watch_start->next==NULL)
        {
            watch_start->next=watch;
            break;
        }
        watch_start=watch_start->next;
    }

}
