
#include "rtconfig.h"
#include "dfs_fs.h"
#include "rtthread.h"
#include "appstack.h"
#include "string"
#include "rc.h"
#ifdef LIB_USING_JSONCPP
#include "json/value.h"
#include "json/writer.h"
#endif // LIB_USING_JSONCPP

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
#ifdef RT_USING_DFS_TMPFS
    {
        if(dfs_mount(NULL,"/","tmp",0,NULL)!=0)
        {
            printf("mount tmpfs on / failed!\r\n");
        }
        else
        {
            printf("mount tmpfs on / success!\r\n");
        }
    }
#endif // RT_USING_DFS_TMPFS
    {
        //打印剩余内存
        rt_size_t total = 0, used = 0;
        rt_memory_info(&total, &used, NULL);
        printf("\r\nTotal Memory:%d Bytes,Used Memory:%d Bytes\r\n", (int)total, (int)used);
    }
#ifdef LIB_USING_JSONCPP
    {
        //测试jsoncpp
        Json::Value value;
        value["name"]="jsoncpp";
        Json::StyledWriter writer;
        printf("json parser:\r\n%s\r\n",writer.write(value).c_str());
    }
#endif // LIB_USING_JSONCPP
}

void App_Loop()
{

}
