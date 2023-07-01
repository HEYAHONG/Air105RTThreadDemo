#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include "rtthread.h"
#include "rtconfig.h"
#include "stdint.h"


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifdef BSP_USING_LED

typedef enum
{
    LED_INDEX_LED0,
    LED_INDEX_LED1,
    LED_INDEX_LED2,
    LED_INDEX_MAX
} led_index_t;


/** \brief 修改LED亮灭表
 *
 * \param i led_index_t LED引索
 * \param table uint64_t 亮灭表,每bit表示100ms，共64bits。
 * \return int 成功返回0
 *
 */
int led_set_table(led_index_t i,uint64_t table);

/** \brief 获取LED亮灭表
 *
 * \param i led_index_t LED引索
 * \return uint64_t LED亮灭表,LED不存在时返回0
 *
 */
uint64_t led_get_table(led_index_t i);


/** \brief 设置LED亮灭
 *
 * \param i led_index_t LED引索
 * \param onoff bool 亮灭(true表示亮)
 * \return int 成功返回0
 *
 */
int led_set_onoff(led_index_t i,bool onoff);

#endif // BSP_USING_LED

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LED_H_INCLUDED
