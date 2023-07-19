/***************************************************************
 * Name:      hobject.h
 * Purpose:   声明hobject接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef HOBJECT_H_INCLUDED
#define HOBJECT_H_INCLUDED

/*
本文件主要用于实现一些基于C语言的简易的对象操作,可配合其它文件使用。
    主要包含以下对象:指针、double,无符号整数，有符号整数
*/


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

typedef enum
{
    HOBJECT_BASE_TYPE_SIMPLE_PTR=1,/**< 简单指针 */
    HOBJECT_BASE_TYPE_ARRAY_PTR,/**< 数组指针 */
    HOBJECT_BASE_TYPE_MANAGED_PTR,/**< 管理的指针(即带释放回调的指针) */
    HOBJECT_BASE_TYPE_MANAGED_ARRAY_PTR,/**< 管理的数组指针(即带释放回调的指针) */
    HOBJECT_BASE_TYPE_DOUBLE,/**< 浮点数 */
    HOBJECT_BASE_TYPE_INT8, /**< 整型(8位) */
    HOBJECT_BASE_TYPE_UINT8, /**< 无符号整型(8位) */
    HOBJECT_BASE_TYPE_INT16, /**< 整型(16位) */
    HOBJECT_BASE_TYPE_UINT16, /**< 无符号整型(16位) */
    HOBJECT_BASE_TYPE_INT32, /**< 整型(32位) */
    HOBJECT_BASE_TYPE_UINT32, /**< 无符号整型(32位) */
    HOBJECT_BASE_TYPE_INT64, /**< 整型(64位) */
    HOBJECT_BASE_TYPE_UINT64, /**< 无符号整型(64位) */
} hobject_base_type_t; /**< 基本对象类型 */

typedef struct
{
    uint16_t type; /**< 类型，类型必须为hobject_base_type_t规定的类型 */
    uint16_t usr_type; /**< 用户类型,由用户自定义 */
} hobject_base_t; /**< 对象基类 */

/** \brief 从对象指针中获取基类指针(仅限由对象基类派生的)
 *
 * \param  对象指针
 * \return 对象基类指针
 *
 */
#define HOBJECT_BASE(obj_ptr) ((hobject_base_t *)(void *)obj_ptr)

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 指针
    *
    */
    void *ptr;

} hobject_simple_ptr_t; /**< SIMPLE_PTR结构体 */

/** \brief hobject_simple_ptr_t初始化
 *
 * \param obj_ptr hobject_simple_ptr_t* hobject_simple_ptr_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param ptr void* 指针
 *
 */
void hobject_simple_ptr_init(hobject_simple_ptr_t *obj_ptr,uint16_t usr_type,void *ptr);

/** \brief  hobject_simple_ptr_t获取基类指针
 *
 * \param obj_ptr hobject_simple_ptr_t* hobject_simple_ptr_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_simple_ptr_base(hobject_simple_ptr_t *obj_ptr);

/** \brief 获取hobject_simple_ptr_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_simple_ptr_t* hobject_simple_ptr_t对象指针,失败返回NULL
 *
 */
hobject_simple_ptr_t * hobject_simple_ptr(hobject_base_t *base);


typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数组首地址
    *
    */
    void *array_header;
    /** \brief 数组大小(即数组成员个数，非数组占用的字节数)
    *
    */
    size_t array_size;

} hobject_array_ptr_t; /**< ARRAY_PTR结构体 */

/** \brief hobject_array_ptr_t初始化
 *
 * \param obj_ptr hobject_array_ptr_t* hobject_array_ptr_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param array_header void* 数组首地址
 * \param array_size size_t 数组大小(即数组成员个数，非数组占用的字节数)
 *
 */
void hobject_array_ptr_init(hobject_array_ptr_t *obj_ptr,uint16_t usr_type,void *array_header,size_t array_size);

/** \brief hobject_array_ptr_t获取基类指针
 *
 * \param obj_ptr hobject_array_ptr_t* hobject_array_ptr_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t *hobject_array_ptr_base(hobject_array_ptr_t *obj_ptr);

/** \brief 获取hobject_array_ptr_t对象指针
 *
 * \param base hobject_base_t* hobject_base_t基类指针
 * \return hobject_array_ptr_t* hobject_array_ptr_t对象指针,失败返回NULL
 *
 */
hobject_array_ptr_t *hobject_array_ptr(hobject_base_t *base);

typedef struct __hobject_managed_ptr
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 指针
    *
    */
    void *ptr;
    /** \brief 释放回调，一般用于清理ptr,一般使用hobject_managed_ptr_cleanup自动调用
    *
    */
    void (*onfree)(struct __hobject_managed_ptr *obj_ptr);
} hobject_managed_ptr_t; /**< MANAGED_PTR结构体 */

/** \brief hobject_managed_ptr_t初始化
 *
 * \param obj_ptr hobject_managed_ptr_t* hobject_managed_ptr_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param ptr void* 指针
 * \param onfree 释放回调
 *
 */
void hobject_managed_ptr_init(hobject_managed_ptr_t *obj_ptr,uint16_t usr_type,void *ptr,void (*onfree)(hobject_managed_ptr_t *obj_ptr));


/** \brief 清理hobject_managed_ptr_t对象
 *
 * \param obj_ptr hobject_managed_ptr_t* hobject_managed_ptr_t对象指针
 *
 */
void hobject_managed_ptr_cleanup(hobject_managed_ptr_t *obj_ptr);

/** \brief hobject_managed_ptr_t获取基类指针
 *
 * \param obj_ptr hobject_managed_ptr_t* hobject_managed_ptr_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t *hobject_managed_ptr_base(hobject_managed_ptr_t *obj_ptr);

/** \brief 获取hobject_managed_ptr_t对象指针
 *
 * \param base hobject_base_t* hobject_base_t基类指针
 * \return hobject_managed_ptr_t* hobject_managed_ptr_t对象指针,失败返回NULL
 *
 */
hobject_managed_ptr_t *hobject_managed_ptr(hobject_base_t *base);

typedef struct __hobject_managed_array_ptr
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数组首地址
    *
    */
    void *array_header;
    /** \brief 数组大小(即数组成员个数，非数组占用的字节数)
    *
    */
    size_t array_size;
    /** \brief 释放回调，一般用于清理array_header,一般使用hobject_managed_array_ptr_cleanup自动调用
    *
    */
    void (*onfree)(struct __hobject_managed_array_ptr *obj_ptr);

} hobject_managed_array_ptr_t; /**< MANAGED_ARRAY_PTR结构体 */

/** \brief hobject_managed_array_ptr_t初始化
 *
 * \param obj_ptr hobject_managed_array_ptr_t* hobject_managed_array_ptr_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param array_header void* 数组首地址
 * \param array_size size_t 数组大小(即数组成员个数，非数组占用的字节数)
 * \param onfree 释放回调
 *
 */
void hobject_managed_array_ptr_init(hobject_managed_array_ptr_t *obj_ptr,uint16_t usr_type,void *array_header,size_t array_size,void (*onfree)(hobject_managed_array_ptr_t *obj_ptr));

/** \brief 清理hobject_managed_array_ptr_t对象
 *
 * \param obj_ptr hobject_managed_array_ptr_t* hobject_managed_array_ptr_t对象
 *
 */
void hobject_managed_array_ptr_cleanup(hobject_managed_array_ptr_t *obj_ptr);

/** \brief hobject_managed_array_ptr_t获取基类指针
 *
 * \param obj_ptr hobject_managed_array_ptr_t* hobject_managed_array_ptr_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t *hobject_managed_array_ptr_base(hobject_managed_array_ptr_t *obj_ptr);

/** \brief 获取hobject_managed_array_ptr_t对象指针
 *
 * \param base hobject_base_t* hobject_base_t基类指针
 * \return hobject_managed_array_ptr_t* hobject_managed_array_ptr_t对象指针,失败返回NULL
 *
 */
hobject_managed_array_ptr_t *hobject_managed_array_ptr(hobject_base_t *base);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HOBJECT_H_INCLUDED
