#include "rtthread.h"
#include "appstack.h"
#include "string"
#include "rc.h"

void App_Init()
{
    {
        printf("AppStack Start!\r\n");
    }
    {
        //打印banner
        char *banner = (char *)RCGetHandle("banner");
        if (banner != NULL)
            printf("%s", banner);
    }

    {
        //打印剩余内存
        rt_size_t total = 0, used = 0;
        rt_memory_info(&total, &used, NULL);
        printf("\r\nTotal Memory:%d Bytes,Used Memory:%d Bytes\r\n", (int)total, (int)used);
    }
}

void App_Loop()
{

}
