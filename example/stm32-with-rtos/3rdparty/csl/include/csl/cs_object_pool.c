#include "cs_object_pool.h"
#include "task.h"
#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 下划线开头均为内部接口, 请勿使用
 */
typedef struct
{
    CS_LIST_NODE_OBJECT;     /*!< 单向链表 */
    bool       _is_recycled; /*!< 是否回收过 */
    cs_list_t* _lst;         /*!< 单向链表上下文 */
    void*      _priv;        /*!< 用户私有数据 */
} _cs_object_pool_link;

static cs_list_t*            _cs_get_usr_lst(void* usr_node);    /*!< 从用户节点中取出链表 */
static cs_list_t*            _cs_get_pool_lst(void* pool_node);  /*!< 从池节点中取出链表 */
static void*                 _cs_pool2usr_node(void* pool_node); /*!< 池节点转换为用户节点 */
static _cs_object_pool_link* _cs_usr2pool_node(void* usr_node);  /*!< 用户节点转换为池节点 */

static _cs_object_pool_link* _cs_usr2pool_node(void* usr_node)
{
    if (usr_node)
    {
        return (_cs_object_pool_link*)(((char*)usr_node) - sizeof(_cs_object_pool_link));
    }
    return NULL;
}

static void* _cs_pool2usr_node(void* pool_node)
{
    if (pool_node)
    {
        return (((char*)pool_node) + sizeof(_cs_object_pool_link));
    }
    return NULL;
}

static cs_list_t* _cs_get_usr_lst(void* usr_node)
{
    if (usr_node)
    {
        return _cs_usr2pool_node(usr_node)->_lst;
    }
    return NULL;
}

static cs_list_t* _cs_get_pool_lst(void* pool_node)
{
    if (pool_node)
    {
        return ((_cs_object_pool_link*)pool_node)->_lst;
    }
    return NULL;
}

void* cs_object_pool_init(const int object_count, const int object_size, void* user_data)
{
    int i = 0;
    cs_list_t* lst = (cs_list_t*)cs_malloc(sizeof(cs_list_t));
    if (!lst)
    {
        return NULL;
    }

    cs_list_init(lst);

    for (; i < object_count; i++)
    {
        _cs_object_pool_link* node = (_cs_object_pool_link*)cs_list_malloc_node(sizeof(_cs_object_pool_link) + object_size);
        if (!node)
        {
            cs_list_uninit(lst);
            cs_free(lst);
            return NULL;
        }

        node->_lst = lst;
        node->_priv = user_data;
        node->_is_recycled = true;
        cs_list_push_back(node->_lst, node);
    }

    return _cs_pool2usr_node(cs_list_peek_front(lst));
}

void cs_object_pool_uninit(void* node)
{
    if (node)
    {
        cs_list_t* lst = _cs_get_usr_lst(node);
        cs_list_uninit(lst);
        cs_free(lst);
    }
}

void* cs_object_pool_obtain(void* node)
{
    _cs_object_pool_link* link = NULL;
    if (node)
    {
        //INTX_DISABLE();
        link = cs_list_pop_front(_cs_get_usr_lst(node));
        //INTX_ENABLE();
        if (link)
        {
            link->_is_recycled = false;
            return _cs_pool2usr_node(link);
        }
    }
    return NULL;
}

void cs_object_pool_recycle(void* node)
{
    _cs_object_pool_link* link = NULL;
    if (!node)
    {
        return;
    }

    link = _cs_usr2pool_node(node);

    if (link->_is_recycled)
    {
        return;
    }

    link->_is_recycled = true;
    taskENTER_CRITICAL();
    cs_list_push_back(_cs_get_pool_lst(link), link);
    taskEXIT_CRITICAL();
}

void* cs_object_pool_get_user_data(void* node)
{
    if (node)
    {
        return _cs_usr2pool_node(node)->_priv;
    }
    return NULL;
}

void* cs_object_pool_head(void* node)
{
    if (node)
    {
        void* obj_node = cs_list_peek_front(_cs_get_usr_lst(node));
        return _cs_pool2usr_node(obj_node);
    }
    return NULL;
}

void* cs_object_pool_next(void* node)
{
    if (node)
    {
        void* obj_node = cs_list_next_node(_cs_usr2pool_node(node));
        return _cs_pool2usr_node(obj_node);
    }
    return NULL;
}

int cs_object_pool_get_count(void* node)
{
    if (node)
    {
        return cs_list_get_count(_cs_get_usr_lst(node));
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
