#ifndef CS_LIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_LIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977

#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CS_LIST_NODE_OBJECT _cs_list_link_t _cs_link /*!< ���Զ���Ľڵ�ṹ��, �����ڳ�Ա��������ǰ���������� */

typedef struct _cs_list_link_tag
{
    struct _cs_list_link_tag *next; /*!< ָ����һ�ڵ� */
} _cs_list_link_t;

typedef struct
{
    struct _cs_list_link_tag* _head;  /*!< ͷ */
    struct _cs_list_link_tag* _tail;  /*!< β */
    int                       _count; /*!< �ڵ���� */
} cs_list_t;

/**
 * @brief ��ʼ��
 * @param [in] lst ������
 * @return �ɹ�����0
 */
int cs_list_init(cs_list_t *lst);

/**
 * @brief ����ʼ������, �ͷ����������ϵĽڵ�
 * @param [in] lst ������
 */
void cs_list_uninit(cs_list_t *lst);

/**
 * @brief ����һ���½ڵ�
 * @param [in] node_size �ڵ��С
 * @return �ڵ��׵�ַ
 * @attention �ǵ�ʹ��cs_list_node_free�����ͷ�
 */
void* cs_list_malloc_node(unsigned int node_size);

/**
 * @brief �ͷ�һ���ڵ�
 * @param [in] node �ڵ�
 * @attention �μ�cs_list_node_malloc
 */
void cs_list_free_node(void* node);

/**
 * @brief �ں󷽲���ڵ�
 * @param [in] lst ����lst
 * @param [in] prev ��Ҫ�������λ��, ��������ڵ�ĺ󷽲���
 * @param [in] node �����������Ľڵ�
 * @return �ɹ�����0
 */
int cs_list_insert(cs_list_t *lst, void *prev, void *node);

/**
 * @brief ���ڵ�׷�ӵ������β��
 * @param [in] lst ������
 * @param [in] node �ڵ��׵�ַ
 * @return �ɹ�����0
 */
int cs_list_push_back(cs_list_t *lst, void *node);

/**
 * @brief ���ڵ���������ͷ��
 * @param [in] lst ������
 * @param [in] node �ڵ��׵�ַ
 * @return �ɹ�����0
 */
int cs_list_push_front(cs_list_t *lst, void *node);

/**
 * @brief ���������ͷ�ڵ�, ���Ƴ���һ���ڵ�
 * @param [in] lst ������
 * @return ������ͷ�ڵ�ĵ�ַ
 */
void* cs_list_pop_front(cs_list_t *lst);

/**
 * @brief ��ȡ����ڵ�ĸ���
 * @param [in] lst ������
 * @return �ڵ�ĸ���
 */
int cs_list_get_count(cs_list_t *lst);

/**
 * @brief ��ȡָ���ڵ��Ӧ����һ���ڵ��ַ
 * @param [in] node ָ���ڵ���׵�ַ
 * @return ��һ���ڵ���׵�ַ
 */
void* cs_list_next_node(void *node);

/**
 * @brief ��ȡ�����ͷ�ڵ�
 * @param [in] lst ������
 * @attention ע��, peek��pop����������, pop�Ὣ�ڵ���������Ƴ�, ��peek����
 * @return ͷ�ڵ��׵�ַ
 */
void* cs_list_peek_front(cs_list_t *lst);

#ifdef __cplusplus
}
#endif

#endif
