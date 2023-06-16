
#include "rtconfig.h"
#include "dfs_fs.h"
#include "rtthread.h"
#include "appstack.h"
#include "string"
#include "RC_cpp.h"
#ifdef  RT_USING_DFS_ROMFS
#include "dfs_file.h"
#include "dfs_fs.h"
#include "dfs_romfs.h"
#endif // RT_USING_DFS_ROMFS
#ifdef LIB_USING_JSONCPP
#include "json/value.h"
#include "json/writer.h"
#endif // LIB_USING_JSONCPP
#include "gui_u8g2.h"

void App_Init()
{
    {
        printf("AppStack Start!\r\n");
    }
    {
        //打印banner
        char *banner = (char *)RCGetFileHandle("/banner");
        if (banner != NULL)
            printf("%s", banner);
    }
    {
        printf("RC List:\r\n");
        std::vector<std::string> List=RCGetFileList();
        for(const std::string &filename:List)
        {
            printf("\t\t%s\r\n",filename.c_str());
        }
    }
    {

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


#ifdef  RT_USING_DFS_ROMFS
    {
        extern const struct romfs_dirent romfs_root;
        std::string mount_point="/";
        {
            //检查挂载点
            struct stat s={0};
            if(dfs_file_stat(mount_point.c_str(),&s)==0)
            {
                mount_point="/rom";
                mkdir(mount_point.c_str(),777);
            };
        }
        if(dfs_mount(NULL,mount_point.c_str(),"rom",0,&romfs_root)!=0)
        {
            printf("mount romfs on %s failed!\r\n",mount_point.c_str());
        }
        else
        {
            printf("mount romfs on %s success!\r\n",mount_point.c_str());
        }
    }
#endif //RT_USING_DFS_ROMFS


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

    gui_u8g2_init();
}

void App_Loop()
{
    gui_u8g2_loop();
}
