/**
 * @file
 * @brief 单向链表
 *
 * 适用于头部大量增加,删除操作, 尾部只支持增加操作, 中间插入操作
 * @author yaoyuliang(765495939@qq.com)
 * @copyright SAIZHI.inc
 * @date 2019-03-29
 */
#include <string.h>
#include "cs_list.h"

#define _cs_cast_void_to_node(x) ((_cs_list_link_t*)(x))

#ifdef __cplusplus
extern "C" {
#endif

int cs_list_init(cs_list_t *lst)
{
    if (lst)
    {
        lst->_head = NULL;
        lst->_tail = NULL;
        lst->_count = 0;
        return 0;
    }
    return -1;
}

void cs_list_uninit(cs_list_t *lst)
{
    _cs_list_link_t *p1 = NULL;
    _cs_list_link_t *p2 = NULL;

    if (!lst)
    {
        return;
    }

    if (lst->_count > 0)
    {
        p1 = lst->_head;
        while (p1 != NULL)
        {
            p2 = p1->next;
            cs_free(p1);
            p1 = p2;
        }
        lst->_count = 0;
        lst->_head = lst->_tail = NULL;
    }
}

void* cs_list_malloc_node(unsigned int node_size)
{
    _cs_list_link_t* link = (_cs_list_link_t*)cs_malloc(node_size);
    if (link)
    {
        memset(link, 0, sizeof(_cs_list_link_t));
        return link;
    }
    return NULL;
}

void cs_list_free_node(void* node)
{
    if (node)
    {
        cs_free(node);
    }
}

int cs_list_insert(cs_list_t *lst, void *prev, void *node)
{
    _cs_list_link_t *next = NULL;

    if (!lst || !node)
    {
        return -1;
    }

    if (prev == NULL)
    {
        next = lst->_head;
        lst->_head = node;
    }
    else
    {
        next = _cs_cast_void_to_node(prev)->next;
        _cs_cast_void_to_node(prev)->next = node;
    }

    if (next == NULL)
    {
        lst->_tail = node;
    }

    _cs_cast_void_to_node(node)->next = next;
    lst->_count++;

    return 0;
}

int cs_list_push_back(cs_list_t *lst, void *node)
{
    return cs_list_insert(lst, lst->_tail, node);
}

int cs_list_push_front(cs_list_t *lst, void *node)
{
    _cs_list_link_t* head = NULL;

    if (!lst || !node)
    {
        return -1;
    }

    head = lst->_head;
    if (head)
    {
        _cs_cast_void_to_node(node)->next = head;
        lst->_head = node;
        lst->_count++;
    }
    else
    {
        cs_list_insert(lst, lst->_tail, node);
    }

    return 0;
}

void* cs_list_pop_front(cs_list_t *lst)
{
    _cs_list_link_t *head = NULL;
    if (!lst)
    {
        return NULL;
    }

    head = lst->_head;

    if (head != NULL)
    {
        lst->_head = head->next;
        if (head->next == NULL)
        {
            lst->_tail = NULL;
        }
        lst->_count--;
    }

    return head;
}

int cs_list_get_count(cs_list_t *lst)
{
    return lst ? (lst->_count) : 0;
}

void* cs_list_next_node(void *node)
{
    return node ? (_cs_cast_void_to_node(node)->next) : NULL;
}

void* cs_list_peek_front(cs_list_t *lst)
{
    return lst ? (lst->_head) : NULL;
}

#ifdef __cplusplus
}
#endif
