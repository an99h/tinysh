#ifndef CS_OBJECT_POOL_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_OBJECT_POOL_H_3935E820_DD02_4DD7_AC55_9F92796BC977

#include "cs_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 对象池初始化
 * @param [in] objectCount 对象池个数
 * @param [in] objectSize  每个对象大小
 * @param [in] private_data 用户的私有数据
 * @return  对象池头节点地址
 */
void* cs_object_pool_init(const int object_count, const int object_size, void* user_data);

/**
 * @brief 对象池反始化
 * @param [in] node 对象池某个节点
 */
void cs_object_pool_uninit(void* node);

/**
 * @brief 获取对象池首节点
 * @param [in] node  对象池某个节点
 * @return 对象池首节点地址
 */
void* cs_object_pool_obtain(void* node);

/**
 * @brief 回收对象池节点
 * @param [in] node 需要收回的对象池节点
 */
void cs_object_pool_recycle(void* node);

/**
 * @brief 获取携带的用户数据
 * @param [in] node 节点
 * @return 返回用户数据
 */
void* cs_object_pool_get_user_data(void* node);

/**
 * @def cs_object_pool_head
 * @brief 获取对象池头节点
 * @param [in] node 对象池节点
 */
void* cs_object_pool_head(void* node);

/**
 * @def cs_object_pool_next
 * @brief 获取对象池node节点下一个节点
 * @param [in] node 对象池节点
 */
void* cs_object_pool_next(void* node);

/**
 * @def cs_object_pool_get_count
 * @brief 获取对象池节点数
 * @param [in] node 对象池节点
 */
int cs_object_pool_get_count(void* node);


#ifdef __cplusplus
}
#endif

#endif
