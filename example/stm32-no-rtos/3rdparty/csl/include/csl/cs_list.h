#ifndef CS_LIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_LIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977

#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CS_LIST_NODE_OBJECT _cs_list_link_t _cs_link /*!< 你自定义的节点结构体, 必须在成员变量的最前面加上这个宏 */

typedef struct _cs_list_link_tag
{
    struct _cs_list_link_tag *next; /*!< 指向下一节点 */
} _cs_list_link_t;

typedef struct
{
    struct _cs_list_link_tag* _head;  /*!< 头 */
    struct _cs_list_link_tag* _tail;  /*!< 尾 */
    int                       _count; /*!< 节点个数 */
} cs_list_t;

/**
 * @brief 初始化
 * @param [in] lst 上下文
 * @return 成功返回0
 */
int cs_list_init(cs_list_t *lst);

/**
 * @brief 反初始化链表, 释放所有链表上的节点
 * @param [in] lst 上下文
 */
void cs_list_uninit(cs_list_t *lst);

/**
 * @brief 创建一个新节点
 * @param [in] node_size 节点大小
 * @return 节点首地址
 * @attention 记得使用cs_list_node_free进行释放
 */
void* cs_list_malloc_node(unsigned int node_size);

/**
 * @brief 释放一个节点
 * @param [in] node 节点
 * @attention 参见cs_list_node_malloc
 */
void cs_list_free_node(void* node);

/**
 * @brief 在后方插入节点
 * @param [in] lst 链表lst
 * @param [in] prev 需要被插入的位置, 将在这个节点的后方插入
 * @param [in] node 待插入进链表的节点
 * @return 成功返回0
 */
int cs_list_insert(cs_list_t *lst, void *prev, void *node);

/**
 * @brief 将节点追加到链表的尾部
 * @param [in] lst 上下文
 * @param [in] node 节点首地址
 * @return 成功返回0
 */
int cs_list_push_back(cs_list_t *lst, void *node);

/**
 * @brief 将节点插入链表的头部
 * @param [in] lst 上下文
 * @param [in] node 节点首地址
 * @return 成功返回0
 */
int cs_list_push_front(cs_list_t *lst, void *node);

/**
 * @brief 弹出链表的头节点, 即移除第一个节点
 * @param [in] lst 上下文
 * @return 弹出的头节点的地址
 */
void* cs_list_pop_front(cs_list_t *lst);

/**
 * @brief 获取链表节点的个数
 * @param [in] lst 上下文
 * @return 节点的个数
 */
int cs_list_get_count(cs_list_t *lst);

/**
 * @brief 获取指定节点对应的下一个节点地址
 * @param [in] node 指定节点的首地址
 * @return 下一个节点的首地址
 */
void* cs_list_next_node(void *node);

/**
 * @brief 获取链表的头节点
 * @param [in] lst 上下文
 * @attention 注意, peek和pop的区别在于, pop会将节点从链表中移除, 而peek不会
 * @return 头节点首地址
 */
void* cs_list_peek_front(cs_list_t *lst);

#ifdef __cplusplus
}
#endif

#endif
