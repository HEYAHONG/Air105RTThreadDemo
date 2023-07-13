#include "heventchain.h"

typedef struct heventchain_hook
{
    //用户参数
    void *usr;
    //钩子函数,第一个参数为传入参数,第二个参数为用户参数
    bool (*hook)(void *,void *);
    //钩子释放,第一个参数为用户参数
    void (*onfree)(void *);
    //下一个钩子
    struct heventchain_hook *next;
    //钩子id
    uint32_t id;
    //优先级
    uint32_t priority;
} heventchain_hook_t;

struct heventchain
{
    //用户参数
    void *usr;
    //获取内存,第一个参数为待获取的内存大小,第二个参数为用户参数。
    void *(*mem_alloc)(size_t,void *);
    //释放内存,第一个参数为待释放的内存指针,第二个参数为用户参数。
    void (*mem_free)(void *,void *);
    //锁,参数为用户参数。
    void (*mutex_lock)(void *);
    //解锁,参数为用户参数。
    void (*mutex_unlock)(void *);
    //槽单向链表指针
    heventchain_hook_t *hook_start;
    //下一个钩子id
    uint32_t hook_next_id;
};

heventchain_t * heventchain_new_with_memmang_and_lock(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *),void (*mutex_lock)(void *),void (*mutex_unlock)(void *))
{
    struct heventchain *chain=NULL;
    if(mem_alloc!=NULL)
    {
        chain=(struct heventchain *)mem_alloc(sizeof(struct heventchain),usr);
    }
    else
    {
        chain=(struct heventchain *)malloc(sizeof(struct heventchain));
    }

    if(chain==NULL)
    {
        return NULL;
    }

    memset(chain,0,sizeof(struct heventchain));

    chain->usr=usr;
    chain->mem_alloc=mem_alloc;
    chain->mem_free=mem_free;
    chain->mutex_lock=mutex_lock;
    chain->mutex_unlock=mutex_unlock;
    chain->hook_start=NULL;
    chain->hook_next_id=1;

    return chain;
}
heventchain_t * heventchain_new_with_memmang(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *))
{
    return heventchain_new_with_memmang_and_lock(usr,mem_alloc,mem_free,NULL,NULL);
}

static void*default_malloc(size_t nBytes,void *usr)
{
    return malloc(nBytes);
}

static void default_free(void *ptr,void *usr)
{
    return free(ptr);
}

heventchain_t * heventchain_new_with_lock(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *))
{
    return heventchain_new_with_memmang_and_lock(usr,default_malloc,default_free,mutex_lock,mutex_unlock);
}
heventchain_t * heventchain_new(void *usr)
{
    return heventchain_new_with_memmang_and_lock(usr,default_malloc,default_free,NULL,NULL);

}

void * heventchain_get_usr_ptr(heventchain_t *chain)
{
    if(chain==NULL)
    {
        return NULL;
    }
    return chain->usr;
}

void heventchain_free(heventchain_t *chain)
{
    if(chain==NULL)
    {
        return;
    }

    heventchain_uninstall_all_hook(chain);

    if(chain->mem_free!=NULL)
    {
        chain->mem_free(chain,chain->usr);
    }
    else
    {
        free(chain);
    }
}

bool heventchain_start(heventchain_t *chain,void *parameter)
{
    bool ret=false;

    if(chain==NULL)
    {
        return ret;
    }

    //加锁
    if(chain->mutex_lock!=NULL)
    {
        chain->mutex_lock(chain->usr);
    }

    heventchain_hook_t *hook=chain->hook_start;

    while(hook!=NULL)
    {
        if(hook->hook!=NULL)
        {
            ret=hook->hook(parameter,hook->usr);
            if(ret)
            {
                //成功处理,不再向后传递
                break;
            }
        }
        hook=hook->next;
    }

    //解锁
    if(chain->mutex_unlock!=NULL)
    {
        chain->mutex_unlock(chain->usr);
    }


    return ret;
}

uint32_t heventchain_install_hook(heventchain_t *chain,uint32_t priority,void *hook_usr,bool (*onhook)(void *,void *),void (*onfree)(void *))
{
    if(chain==NULL)
    {
        return 0;
    }

    heventchain_hook_t *hook=NULL;

    if(chain->mem_alloc!=NULL)
    {
        hook=(heventchain_hook_t *)chain->mem_alloc(sizeof(heventchain_hook_t),chain->usr);
    }
    else
    {
        hook=(heventchain_hook_t *)malloc(sizeof(heventchain_hook_t));
    }

    if(hook==NULL)
    {
        return 0;
    }

    hook->usr=hook_usr;
    hook->hook=onhook;
    hook->onfree=onfree;
    hook->id=chain->hook_next_id++;
    hook->priority=priority;
    hook->next=NULL;

    //加锁
    if(chain->mutex_lock!=NULL)
    {
        chain->mutex_lock(chain->usr);
    }

    heventchain_hook_t *chain_hook=chain->hook_start;

    while(chain_hook!=NULL)
    {
        if(chain_hook->next==NULL)
        {
            chain_hook->next=hook;
            break;
        }
        if(hook->priority==chain_hook->next->priority)
        {
            //优先级相等，插入链表
            hook->next=chain_hook->next;
            chain_hook->next=hook;
            break;
        }
        chain_hook=chain_hook->next;
    }

    if(chain_hook==NULL)
    {
        chain->hook_start=hook;
    }

    //解锁
    if(chain->mutex_unlock!=NULL)
    {
        chain->mutex_unlock(chain->usr);
    }

    return hook->id;
}

void heventchain_uninstall_hook(heventchain_t *chain,uint32_t id)
{
    f(chain==NULL)
    {
        return;
    }

    //加锁
    if(chain->mutex_lock!=NULL)
    {
        chain->mutex_lock(chain->usr);
    }

    heventchain_hook_t *chain_hook=chain->hook_start;

    if(chain_hook->id==id)
    {
        if(chain_hook->onfree!=NULL)
        {
            chain_hook->onfree(chain_hook->usr);
        }
        if(chain->mem_free!=NULL)
        {
            chain->mem_free(chain_hook,chain->usr);
        }
        else
        {
            free(chain_hook);
        }
        chain->hook_start=chain_hook->next;
        chain_hook=NULL;
    }

    while(chain_hook!=NULL)
    {
        if(chain_hook->next==NULL)
        {
            break;
        }

        if(chain_hook->next->id==id)
        {
            heventchain_hook_t *hook=chain_hook->next;
            chain_hook->next=chain_hook->next->next;
            if(hook->onfree!=NULL)
            {
                hook->onfree(hook->usr);
            }
            if(chain->mem_free!=NULL)
            {
                chain->mem_free(hook,chain->usr);
            }
            else
            {
                free(hook);
            }
            break;
        }

        chain_hook=chain_hook->next;
    }


    //解锁
    if(chain->mutex_unlock!=NULL)
    {
        chain->mutex_unlock(chain->usr);
    }

}

void heventchain_uninstall_all_hook(heventchain_t *chain)
{
    if(chain==NULL)
    {
        return;
    }

    //加锁
    if(chain->mutex_lock!=NULL)
    {
        chain->mutex_lock(chain->usr);
    }

    heventchain_hook_t *hook=chain->hook_start;
    chain->hook_start=NULL;
    while(hook!=NULL)
    {
        heventchain_hook_t *next_hook=hook->next;
        if(hook->onfree!=NULL)
        {
            hook->onfree(hook->usr);
        }
        if(chain->mem_free!=NULL)
        {
            chain->mem_free(hook,chain->usr);
        }
        else
        {
            free(hook);
        }
        hook=next_hook;
    }

    chain->hook_next_id=1;

    //解锁
    if(chain->mutex_unlock!=NULL)
    {
        chain->mutex_unlock(chain->usr);
    }

}
