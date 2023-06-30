#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "rtthread.h"
#include "rtconfig.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum
{
    KEY_EVENT_TYPE_ON_PRESS,/**< 按键处于按压状态 */
    KEY_EVENT_TYPE_ON_RELEASE,/**< 按键处于松开状态 */
    KEY_EVENT_TYPE_CLICK,/**< 按键单击 */
    KEY_EVENT_TYPE_DOUBLE_CLICK /**< 按键双击 */,
    KEY_EVENT_TYPE_LONG_PRESS /**< 按键长按 */
} key_event_type_t;

typedef enum
{
    KEY_INDEX_KEY0=0,
    KEY_INDEX_MAX
} key_index_t;

typedef void(*key_event_callback_t)(key_index_t i,key_event_type_t type,void *usr);

/** \brief 设置按键处理函数
 *
 * \param cb key_event_callback_t 按键事件处理函数
 * \param usr void* 用户参数
 * \return int 0表示成功
 *
 */
int key_set_event_handler(key_event_callback_t cb,void* usr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // KEY_H_INCLUDED
