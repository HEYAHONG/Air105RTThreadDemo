/***************************************************************
 * Name:      hwatchdog.h
 * Purpose:   声明hwatchdog接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef HWATCHDOG_H_INCLUDED
#define HWATCHDOG_H_INCLUDED

/*
本文件用于实现辅助看门狗操作。
    看门狗通常只能启动(添加),不能通过软件方法停止。
    本文件中看门狗分为软件看门狗与硬件看门狗。
        -软件看门狗通常使用系统节拍作为依据，当检查到异常状态超时时调用系统复位。
        -硬件看门狗通常是一个定时器，当长时间不软件喂狗时,由定时器触发复位。通常不可关闭。
    使用本文件的一般步骤如下:
        -设置看门狗内存管理(可选)。
        -设置软件/硬件看门狗,视看门狗的设置情况不同行为也略有不同。
        -在空闲任务(若有)中喂狗,若不包含操作系统，则需要在主循环中添加喂狗。
        -在需要检查的函数中添加检查。
*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

/** \brief 设置看门狗内存管理。如不设置将使用malloc与free。
 *
 * \param  usr void* 用户指针
 * \param  mem_alloc 内存分配函数,第一个参数是待分配的字节数，第二个参数是用户指针
 * \param  mem_free  内存释放函数,第一个参数是已分配的指针，第二个参数是用户指针
 * \param  mutex_lock 加锁,第一个参数是用户指针
 * \param  mutex_unlock 解锁,第一个参数是用户指针
 */
void hwatchdog_set_memmang_and_lock(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *),void (*mutex_lock)(void *),void (*mutex_unlock)(void *));

/** \brief 获取看门狗用户指针
 *
 * \return void* 看门狗用户指针
 *
 */
void *hwatchdog_get_usr_ptr();

/** \brief 设置硬件看门狗
 *
 * \param hw_feed 硬件看门狗喂狗
 *
 */
void hwatchdog_set_hardware_dog_feed(void (*hw_feed)());

/** \brief 设置软件看门狗(调用hwatchdog_feed检查条件,超时时调用复位函数复位),若未设置软件看门狗,则条件不成立时立即判定失败。
 *
 * \param sys_reset 系统复位
 * \param sys_tick_ms 系统节拍(毫秒),通常为自增值
 *
 */
void hwatchdog_setup_software_dog(void (*sys_reset)(),uint64_t (*sys_tick_ms)());

/** \brief 检查看门狗是否有效，若无效则说明某些必要参数未设置。
 *
 * \return bool 是否有效.
 *
 */
bool hwatchdog_is_valid();

/** \brief 看门狗喂狗。
 * 注意:若未设置硬件看门狗且需要使用软件看门狗,则需要确保喂狗一定能得到执行。
 *      若使用操作系统，通常在空闲任务中得到调用。
 *
 *
 */
void hwatchdog_feed();


/** \brief 喂狗
 *
 *
 */
#define HWATCHDOG_FEED() hwatchdog_feed()

typedef struct
{
    void *usr;
    const char *file;
    int line;
    const char *fuction;
} hwatchdog_watch_info_t;

/** \brief 添加看门狗检查(一般不直接使用此函数,而使用HWATCHDOG_ADD_WATCH)
 *
 * \param check 检查函数。检查失败返回false(即异常状态)
 * \param timeout_ms uint32_t 超时。仅当软件看门狗正确设置后有效。
 * \param info hwatchdog_watch_info_t检查信息
 *
 */
void hwatchdog_add_watch(bool (*check)(hwatchdog_watch_info_t* info),uint32_t timeout_ms,hwatchdog_watch_info_t info);

/** \brief 添加看门狗检查
 *
 * \param check 检查函数。检查失败返回false(即异常状态)
 * \param timeout_ms 超时。仅当软件看门狗正确设置后有效
 * \param usr 用户参数
 *
 */
#define HWATCHDOG_ADD_WATCH(check,timeout_ms,usr) {hwatchdog_watch_info_t info={usr,__FILE__,__LINE__,__FUNCTION__};hwatchdog_add_watch(check,timeout_ms,info);}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HWATCHDOG_H_INCLUDED
