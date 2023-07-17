/***************************************************************
 * Name:      hmemoryheap.c
 * Purpose:   实现hmemoryheap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hmemoryheap.h"

struct hmemoryheap_pool
{
    //魔数,用于标记是否初始化过。未初始化过的内存池不可用。
    uint32_t magic;
    //总大小(表示块所占空间的总大小,不包含struct hmemoryheap_pool所占大小)
    uint32_t totalsize;
    //空闲大小,空闲大小并不能决定是否能成功分配,要看是否有足够大的空闲块
    uint32_t freesize;
    //用户参数
    void * usr;
    //锁,参数为用户参数。
    void (*mutex_lock)(void *);
    //解锁,参数为用户参数。
    void (*mutex_unlock)(void *);
    //异常回调,如OOM。
    void (*onexception)(struct hmemoryheap_pool *pool,hmemoryheap_exception_type_t excetion);
} ;

typedef struct
{
    //下一块内存块相对于hmemoryheap_pool指针的偏移字节数
    uint32_t next_block_offset:31;
    //当前块是否是空闲块。1等于空闲。
    uint32_t blockfree:1;

} hmemoryheap_pool_block_t;

/** \brief 将大小扩大为对齐大小
 *
 * \param size uint64_t 对齐前的大小
 * \return uint64_t 对齐后的大小
 *
 */
static uint64_t expand_size_to_aligned_size(uint64_t size)
{
    uint64_t ret=size/HMEMORYHEAP_ALIGNED_SIZE*HMEMORYHEAP_ALIGNED_SIZE;

    if(ret%HMEMORYHEAP_ALIGNED_SIZE!=0)
    {
        ret+=HMEMORYHEAP_ALIGNED_SIZE;
    }

    return ret;
}


/** \brief 将大小缩小为对齐大小
 *
 * \param size uint64_t 对齐前的大小
 * \return uint64_t 对齐后的大小
 *
 */
static uint64_t shrink_size_to_aligned_size(uint64_t size)
{
    return size/HMEMORYHEAP_ALIGNED_SIZE*HMEMORYHEAP_ALIGNED_SIZE;
}


/** \brief 指针大小,若出现警告则应当调整此值，一般是4（32位系统）或8（64位系统）
 *
 *
 */
#ifdef HMEMORYHEAP_PTR_SIZE
#define PTR_SIZE  HMEMORYHEAP_PTR_SIZE
#else
#define PTR_SIZE 4
#endif // HMEMORYHEAP_PTR_SIZE


static uint64_t ptr_to_number(void *ptr)
{
#if PTR_SIZE == 4
    {
        return (uint32_t)ptr;
    }
#else
    {
        return (uint64_t)ptr;
    }
#endif // VOID_PTR_SIZE
}

static void * number_to_ptr(uint64_t number)
{
#if PTR_SIZE == 4
    {
        return (void *)(uint32_t)number;
    }
#else
    {
        return (void *)number;
    }
#endif // PTR_SIZE

}

/** \brief 获取hmemoryheap_pool(结构体)占用大小
 *
 * \return uint64_t hmemoryheap_pool(结构体)占用大小（字节数）
 *
 */
static uint64_t get_hmemoryheap_pool_size()
{
    return expand_size_to_aligned_size(sizeof(struct  hmemoryheap_pool));
}

/** \brief 获取hmemoryheap_pool_block_t(结构体)占用大小
 *
 * \return uint64_t hmemoryheap_pool_block_t(结构体)占用大小
 *
 */
static uint64_t get_hmemoryheap_pool_block_size()
{
    return expand_size_to_aligned_size(sizeof(hmemoryheap_pool_block_t));
}

hmemoryheap_pool_t *hmemoryheap_pool_format(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *),uint8_t *pool_store,size_t pool_store_size)
{
    if(pool_store==NULL || pool_store_size == 0)
    {
        //必须指定数组及大小
        return NULL;
    }

    //起始地址
    uint64_t pool_start_addr=ptr_to_number(pool_store);
    //结束地址
    uint64_t pool_end_addr=pool_start_addr+pool_store_size;

    {
        //丢弃数组头尾的非对齐部分
        pool_start_addr=expand_size_to_aligned_size(pool_start_addr);
        pool_end_addr=shrink_size_to_aligned_size(pool_end_addr);
    }

    uint32_t pool_size=pool_end_addr-pool_start_addr;

    if((pool_end_addr <= pool_start_addr) || (pool_size < (get_hmemoryheap_pool_size()+get_hmemoryheap_pool_block_size())))
    {
        //数组太小
        return NULL;
    }

    if(pool_size > 0x80000000)
    {
        //数组太大,调整大小
        pool_size=shrink_size_to_aligned_size(0x7FFFFFFF);
    }

    pool_end_addr=pool_start_addr+pool_size;

    struct hmemoryheap_pool *pool=(struct hmemoryheap_pool *)number_to_ptr(pool_start_addr);

    if(pool->magic == HMEMORYHEAP_MAGIC_NUMBER)
    {
        //已初始化
        return pool;
    }

    pool->usr=usr;
    pool->mutex_lock=mutex_lock;
    pool->mutex_unlock=mutex_unlock;
    pool->magic=HMEMORYHEAP_MAGIC_NUMBER;
    pool->totalsize=pool_size-get_hmemoryheap_pool_size();
    pool->freesize=pool->totalsize;
    pool->onexception=NULL;

    {
        //初始化第一块内存为空闲
        uint64_t first_block_addr=pool_start_addr+get_hmemoryheap_pool_size();
        hmemoryheap_pool_block_t * block=(hmemoryheap_pool_block_t *)number_to_ptr(first_block_addr);
        block->next_block_offset=pool_size;
        block->blockfree=1;
    }

    return pool;
}

void * hmemoryheap_pool_get_usr_ptr(hmemoryheap_pool_t *pool)
{
    if(pool==NULL)
    {
        return NULL;
    }

    return pool->usr;
}

void  hmemoryheap_pool_set_exception(hmemoryheap_pool_t *pool,void (*onexception)(struct hmemoryheap_pool *pool,hmemoryheap_exception_type_t excetion))
{
    if(pool!=NULL)
    {
        pool->onexception=onexception;
    }
}

void * hmemoryheap_pool_malloc(hmemoryheap_pool_t *pool,size_t nbytes)
{
    if(pool==NULL || pool->magic!=HMEMORYHEAP_MAGIC_NUMBER)
    {
        //池不存在或未初始化
        return NULL;
    }

    uint64_t pool_start_addr=ptr_to_number(pool);
    uint64_t pool_end_addr=pool_start_addr+pool->totalsize+get_hmemoryheap_pool_size();
    size_t wantedsize=expand_size_to_aligned_size(nbytes);
    if(wantedsize > pool->freesize)
    {
        //剩余空间不够
        return NULL;
    }
    hmemoryheap_pool_block_t * block;
    {
        uint64_t first_block_addr=pool_start_addr+get_hmemoryheap_pool_size();
        block=(hmemoryheap_pool_block_t *)number_to_ptr(first_block_addr);
    }

    void *ret=NULL;
    //加锁
    if(pool->mutex_lock!=NULL)
    {
        pool->mutex_lock(pool->usr);
    }

    while(true)
    {
        uint64_t block_start_addr=ptr_to_number(block);
        uint64_t next_block_start_addr=block->next_block_offset+pool_start_addr;
        if(block->blockfree==1)
        {
            //空闲块
            while(true)
            {
                //合并空闲内存块
                if(next_block_start_addr>=pool_end_addr)
                {
                    //已到池末尾
                    break;
                }
                hmemoryheap_pool_block_t * next_block=(hmemoryheap_pool_block_t *)number_to_ptr(next_block_start_addr);
                if(next_block->blockfree == 1)
                {
                    //下一个是空闲块,合并块
                    block->next_block_offset=next_block->next_block_offset;
                    next_block_start_addr=block->next_block_offset+pool_start_addr;
                }
                else
                {
                    break;
                }
            }

            if((next_block_start_addr > block_start_addr)&&((next_block_start_addr-block_start_addr) >= (get_hmemoryheap_pool_block_size()+wantedsize)))
            {
                //空闲空间够用
                if(((next_block_start_addr-block_start_addr) > (get_hmemoryheap_pool_block_size()*2+wantedsize)))
                {
                    block->blockfree=0;
                    ret=number_to_ptr(block_start_addr+get_hmemoryheap_pool_block_size());
                    uint64_t new_block_start_addr=block_start_addr+get_hmemoryheap_pool_block_size()+wantedsize;
                    pool->freesize-=(new_block_start_addr-block_start_addr);
                    //创建新块
                    hmemoryheap_pool_block_t * new_block=(hmemoryheap_pool_block_t *)number_to_ptr(new_block_start_addr);
                    new_block->blockfree=1;
                    new_block->next_block_offset=block->next_block_offset;
                    block->next_block_offset=(new_block_start_addr-pool_start_addr);
                    break;
                }
                else
                {
                    //剩余空间不够创建块结构体,不能创建新块
                    block->blockfree=0;
                    pool->freesize-=(next_block_start_addr-block_start_addr);
                    ret=number_to_ptr(block_start_addr+get_hmemoryheap_pool_block_size());
                    break;
                }
            }
        }
        if(next_block_start_addr>=pool_end_addr)
        {
            //已到池末尾
            break;
        }
        block=(hmemoryheap_pool_block_t *)number_to_ptr(next_block_start_addr);
    }

    //解锁
    if(pool->mutex_unlock!=NULL)
    {
        pool->mutex_unlock(pool->usr);
    }

    //清空已申请的内存块
    if(ret!=NULL)
    {
        memset(ret,0,nbytes);
    }
    else
    {
        if(pool->onexception!=NULL)
        {
            pool->onexception(pool,HMEMORYHEAP_EXCEPTION_OOM);
        }
    }

    return ret;
}

bool hmemoryheap_is_ptr_in_pool(hmemoryheap_pool_t *pool,void *ptr)
{
    if(pool==NULL || pool->magic!=HMEMORYHEAP_MAGIC_NUMBER)
    {
        //池不存在或未初始化
        return false;
    }

    uint64_t pool_start_addr=ptr_to_number(pool);
    uint64_t pool_end_addr=pool_start_addr+pool->totalsize+get_hmemoryheap_pool_size();
    uint64_t ptr_addr=ptr_to_number(ptr);
    if(ptr_addr > pool_start_addr && ptr_addr < pool_end_addr)
    {
        return true;
    }
    return false;
}


void hmemoryheap_pool_free(hmemoryheap_pool_t *pool,void *ptr)
{
    if(!hmemoryheap_is_ptr_in_pool(pool,ptr))
    {
        return;
    }

    uint64_t pool_start_addr=ptr_to_number(pool);
    uint64_t pool_end_addr=pool_start_addr+pool->totalsize+get_hmemoryheap_pool_size();
    uint64_t ptr_addr=ptr_to_number(ptr);
    hmemoryheap_pool_block_t * block;
    {
        uint64_t first_block_addr=pool_start_addr+get_hmemoryheap_pool_size();
        block=(hmemoryheap_pool_block_t *)number_to_ptr(first_block_addr);
    }

    bool is_free=false;

    //加锁
    if(pool->mutex_lock!=NULL)
    {
        pool->mutex_lock(pool->usr);
    }

    while(true)
    {
        uint64_t block_start_addr=ptr_to_number(block);
        uint64_t next_block_start_addr=block->next_block_offset+pool_start_addr;

        if(ptr_addr == (block_start_addr+get_hmemoryheap_pool_block_size()))
        {
            //已找到指针所在的块
            block->blockfree=1;
            pool->freesize+=(next_block_start_addr-block_start_addr);
            is_free=true;
            break;
        }

        if(next_block_start_addr>=pool_end_addr)
        {
            //已到池末尾
            break;
        }
        block=(hmemoryheap_pool_block_t *)number_to_ptr(next_block_start_addr);
    }

    //解锁
    if(pool->mutex_unlock!=NULL)
    {
        pool->mutex_unlock(pool->usr);
    }

    if(!is_free)
    {
        if(pool->onexception!=NULL)
        {
            pool->onexception(pool,HMEMORYHEAP_EXCEPTION_DOUBLE_FREE);
        }
    }

}