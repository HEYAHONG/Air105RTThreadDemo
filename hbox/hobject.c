/***************************************************************
 * Name:      hobject.c
 * Purpose:   实现hobject接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hobject.h"

void hobject_cleanup(hobject_base_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        if(obj_ptr->type==HOBJECT_BASE_TYPE_MANAGED_PTR)
        {
            hobject_managed_ptr_cleanup((hobject_managed_ptr_t *)(void*)obj_ptr);
        }
        if(obj_ptr->type==HOBJECT_BASE_TYPE_MANAGED_ARRAY_PTR)
        {
            hobject_managed_array_ptr_cleanup((hobject_managed_array_ptr_t *)(void *)obj_ptr);
        }
    }
}

void hobject_simple_ptr_init(hobject_simple_ptr_t *obj_ptr,uint16_t usr_type,void *ptr)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_SIMPLE_PTR;
        base->usr_type=usr_type;
        obj_ptr->ptr=ptr;
    }
}

hobject_base_t * hobject_simple_ptr_base(hobject_simple_ptr_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_simple_ptr_t * hobject_simple_ptr(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_SIMPLE_PTR)
        {
            return (hobject_simple_ptr_t *)(void*)base;
        }
    }
    return NULL;
}

void hobject_array_ptr_init(hobject_array_ptr_t *obj_ptr,uint16_t usr_type,void *array_header,size_t array_size)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_ARRAY_PTR;
        base->usr_type=usr_type;
        obj_ptr->array_header=array_header;
        obj_ptr->array_size=array_size;
    }
}

hobject_base_t *hobject_array_ptr_base(hobject_array_ptr_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_array_ptr_t *hobject_array_ptr(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_ARRAY_PTR)
        {
            return (hobject_array_ptr_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_managed_ptr_init(hobject_managed_ptr_t *obj_ptr,uint16_t usr_type,void *ptr,void (*onfree)(hobject_managed_ptr_t *obj_ptr))
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_MANAGED_PTR;
        base->usr_type=usr_type;
        obj_ptr->ptr=ptr;
        obj_ptr->onfree=onfree;
    }
}

void hobject_managed_ptr_cleanup(hobject_managed_ptr_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        if(obj_ptr->onfree!=NULL)
        {
            obj_ptr->onfree(obj_ptr);
        }
    }
}

hobject_base_t *hobject_managed_ptr_base(hobject_managed_ptr_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_managed_ptr_t *hobject_managed_ptr(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_MANAGED_PTR)
        {
            return (hobject_managed_ptr_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_managed_array_ptr_init(hobject_managed_array_ptr_t *obj_ptr,uint16_t usr_type,void *array_header,size_t array_size,void (*onfree)(hobject_managed_array_ptr_t *obj_ptr))
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_MANAGED_ARRAY_PTR;
        base->usr_type=usr_type;
        obj_ptr->array_header=array_header;
        obj_ptr->array_size=array_size;
        obj_ptr->onfree=onfree;
    }
}

void hobject_managed_array_ptr_cleanup(hobject_managed_array_ptr_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        if(obj_ptr->onfree!=NULL)
        {
            obj_ptr->onfree(obj_ptr);
        }
    }
}

hobject_base_t *hobject_managed_array_ptr_base(hobject_managed_array_ptr_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_managed_array_ptr_t *hobject_managed_array_ptr(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_MANAGED_ARRAY_PTR)
        {
            return (hobject_managed_array_ptr_t *)(void *)base;
        }
    }
    return NULL;
}

