#include <rtthread.h>
#include <board.h>
#include "drv_gpio.h"
#include "stdint.h"
#include "stdbool.h"

#ifdef  BSP_USING_LED

/* defined the LED0 pin: PD14 */
#define LED0_PIN    GPIOD_14
/* defined the LED1 pin: PD15 */
#define LED1_PIN    GPIOD_15
/* defined the LED2 pin: PC3 */
#define LED2_PIN    GPIOC_03

#ifdef BSP_USING_LED_LED0
//LED0亮灭表,每bit表示100ms
static uint64_t led0_table=0xF0F0F0F0F0F0F0F0;
#endif // BSP_USING_LED_LED0

#ifdef BSP_USING_LED_LED1
//LED1亮灭表,每bit表示100ms
static uint64_t led1_table=0xFF00FF00FF00FF00;
#endif // BSP_USING_LED_LED1

#ifdef BSP_USING_LED_LED2
//LED2亮灭表,每bit表示100ms
static uint64_t led2_table=0xFFFF0000FFFF0000;
#endif // BSP_USING_LED_LED2

static void led_timer_timeout(void *para)
{
    static uint8_t count=0;
    if(count>=64)
    {
        count=0;
    }
    uint64_t mask=(1ULL<<count);

#ifdef BSP_USING_LED_LED0
    if(mask&led0_table)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
    }
    else
    {
        rt_pin_write(LED0_PIN, PIN_LOW);
    }
#endif // BSP_USING_LED_LED0

#ifdef BSP_USING_LED_LED1
    if(mask&led1_table)
    {
        rt_pin_write(LED1_PIN, PIN_HIGH);
    }
    else
    {
        rt_pin_write(LED1_PIN, PIN_LOW);
    }
#endif // BSP_USING_LED_LED1

#ifdef BSP_USING_LED_LED2
    if(mask&led2_table)
    {
        rt_pin_write(LED2_PIN, PIN_HIGH);
    }
    else
    {
        rt_pin_write(LED2_PIN, PIN_LOW);
    }
#endif // BSP_USING_LED_LED1

    count++;
}

static int led_init()
{
    bool hasled=false;

#ifdef BSP_USING_LED_LED0
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    hasled=true;
#endif // BSP_USING_LED_LED0

#ifdef BSP_USING_LED_LED1
    /* set LED1 pin mode to output */
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
    hasled=true;
#endif // BSP_USING_LED_LED1

#ifdef BSP_USING_LED_LED2
    /* set LED2 pin mode to output */
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    hasled=true;
#endif // BSP_USING_LED_LED2

    if(hasled)
    {
        static rt_timer_t timer=NULL;
        timer=rt_timer_create("led",led_timer_timeout,&timer,RT_TICK_PER_SECOND*100/1000,RT_TIMER_FLAG_SOFT_TIMER|RT_TIMER_FLAG_PERIODIC);
        if(timer!=NULL)
        {
            rt_timer_start(timer);
        }
    }


    return RT_EOK;
}

INIT_ENV_EXPORT(led_init);

#endif // BSP_USING_LED
