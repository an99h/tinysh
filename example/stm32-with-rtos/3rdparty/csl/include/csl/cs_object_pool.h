#ifndef CS_OBJECT_POOL_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_OBJECT_POOL_H_3935E820_DD02_4DD7_AC55_9F92796BC977

#include "cs_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ����س�ʼ��
 * @param [in] objectCount ����ظ���
 * @param [in] objectSize  ÿ�������С
 * @param [in] private_data �û���˽������
 * @return  �����ͷ�ڵ��ַ
 */
void* cs_object_pool_init(const int object_count, const int object_size, void* user_data);

/**
 * @brief ����ط�ʼ��
 * @param [in] node �����ĳ���ڵ�
 */
void cs_object_pool_uninit(void* node);

/**
 * @brief ��ȡ������׽ڵ�
 * @param [in] node  �����ĳ���ڵ�
 * @return ������׽ڵ��ַ
 */
void* cs_object_pool_obtain(void* node);

/**
 * @brief ���ն���ؽڵ�
 * @param [in] node ��Ҫ�ջصĶ���ؽڵ�
 */
void cs_object_pool_recycle(void* node);

/**
 * @brief ��ȡЯ�����û�����
 * @param [in] node �ڵ�
 * @return �����û�����
 */
void* cs_object_pool_get_user_data(void* node);

/**
 * @def cs_object_pool_head
 * @brief ��ȡ�����ͷ�ڵ�
 * @param [in] node ����ؽڵ�
 */
void* cs_object_pool_head(void* node);

/**
 * @def cs_object_pool_next
 * @brief ��ȡ�����node�ڵ���һ���ڵ�
 * @param [in] node ����ؽڵ�
 */
void* cs_object_pool_next(void* node);

/**
 * @def cs_object_pool_get_count
 * @brief ��ȡ����ؽڵ���
 * @param [in] node ����ؽڵ�
 */
int cs_object_pool_get_count(void* node);


#ifdef __cplusplus
}
#endif

#endif
