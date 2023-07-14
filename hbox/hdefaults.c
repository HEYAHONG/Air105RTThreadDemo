/***************************************************************
 * Name:      hdefaults.h
 * Purpose:   实现一些一些默认函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hcompiler.h"
#include "hdefaults.h"
#ifdef __RTTHREAD__
#include "rtthread.h"
#endif // __RTTHREAD__

void * hdefaults_malloc(size_t nBytes,void *usr)
{

#ifdef __RTTHREAD__
    return rt_malloc(nBytes);
#else
    return malloc(nBytes);
#endif
}

void hdefaults_free(void *ptr,void *usr)
{
#ifdef __RTTHREAD__
    rt_free(ptr);
#else
    free(ptr);
#endif

}

void  hdefaults_mutex_lock(void *usr)
{
#ifdef  __RTTHREAD__
    rt_enter_critical();
#endif
}

void  hdefaults_mutex_unlock(void *usr)
{
#ifdef  __RTTHREAD__
    rt_exit_critical();
#endif
}
