#ifndef HEVENTLOOP_H_INCLUDED
#define HEVENTLOOP_H_INCLUDED
#ifdef __cplusplus

/*
本文件用于辅助处理事件循环,封装一些常用功能(如使用单向链表添加事件)
*/

extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

struct heventloop;
typedef struct heventloop heventloop_t;

heventloop_t * heventloop_new_with_memmang_and_lock(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *),void (*mutex_lock)(void *),void (*mutex_unlock)(void *));
heventloop_t * heventloop_new_with_memmang(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *));
heventloop_t * heventloop_new_with_lock(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *));
heventloop_t * heventloop_new(void *usr);

void heventloop_free(heventloop_t *loop);

bool heventloop_has_events(heventloop_t *loop);
void heventloop_process_event(heventloop_t *loop);
bool heventloop_add_event(heventloop_t *loop,void *event_usr,void(*event_process)(void *),void(*event_onfree)(void *));




#ifdef __cplusplus
}
#endif // __cplusplus
#endif // HEVENTLOOP_H_INCLUDED
