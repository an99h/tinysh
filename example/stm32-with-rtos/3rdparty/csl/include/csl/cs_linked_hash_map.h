#ifndef CS_HASH_MAP_H_80A595AD_CC31_4BD3_8FE5_A8432889D71C
#define CS_HASH_MAP_H_80A595AD_CC31_4BD3_8FE5_A8432889D71C

#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t(*cs_linkedhashmap_hash_function_t)(const void* input);
typedef unsigned int(*cs_linkedhashmap_is_key_equal_function_t)(const void* lhs, const void* rhs);

typedef struct cs_linkedhashmap_bucket_tag
{
    void* key;      /*!< �� */
    void* val;      /*!< ֵ */
    /* ����Ϊ�ڲ��ֶ�, ����ʹ�� */
    int   _is_used; /*!< 1��ʾ��Ͱ�Ѿ���������, 0��ʾ��ͰΪ��,δ��ʹ�� */
    struct cs_linkedhashmap_bucket_tag* _hash_next;
    struct cs_linkedhashmap_bucket_tag* _lst_next;
    struct cs_linkedhashmap_bucket_tag* _lst_prev;
} cs_linkedhashmap_bucket_t;

typedef struct
{
    cs_linkedhashmap_hash_function_t         _pfn_calc_hashcode;  /*!< hash���� */
    cs_linkedhashmap_is_key_equal_function_t _pfn_is_key_equal;   /*!< ֵ�ȽϺ��� */
    int _array_count; /*!< �����С */
    int _elem_count;  /*!< Ԫ���ܸ��� */
    cs_linkedhashmap_bucket_t* _buckets;
    cs_linkedhashmap_bucket_t* _lst_head;
    cs_linkedhashmap_bucket_t* _lst_tail;
} cs_linkedhashmap_t;

#define cs_linkedhashmap_begin(linkedhashmap)                           ((linkedhashmap)->_lst_head)
#define cs_linkedhashmap_end(linkedhashmap)                             ((linkedhashmap)->_lst_tail)
#define cs_linkedhashmap_next(bucket)                                   ((bucket)->_lst_next)
#define cs_linkedhashmap_previous(bucket)                               ((bucket)->_lst_prev)
#define cs_linkedhashmap_get_count(linkedhashmap)                       ((linkedhashmap)->_elem_count)
#define cs_linkedhashmap_set_hash_function(linkedhashmap, pfunction)    ((linkedhashmap) ? (linkedhashmap)->_pfn_calc_hashcode = (pfunction) : 0)
#define cs_linkedhashmap_set_compare_function(linkedhashmap, pfunction) ((linkedhashmap) ? (linkedhashmap)->_pfn_is_key_equal = (pfunction) : 0)

int   cs_linkedhashmap_init(cs_linkedhashmap_t* linkedhashmap, size_t array_count);
void  cs_linkedhashmap_uninit(cs_linkedhashmap_t* linkedhashmap);
int   cs_linkedhashmap_insert(cs_linkedhashmap_t* linkedhashmap, void* key, void* value);
bool  cs_linkedhashmap_is_exist(cs_linkedhashmap_t* linkedhashmap, const void* key);
void* cs_linkedhashmap_get(cs_linkedhashmap_t* linkedhashmap, const void* key);
void  cs_linkedhashmap_erase(cs_linkedhashmap_t* linkedhashmap, void* key);

/* �ṩ����Ĭ�ϵ�hashֵ�㷨 */
size_t cs_linkedhashmap_calc_int_hashcode(const void* input);           /* �����㷨, ���ֱ�� */
size_t cs_linkedhashmap_calc_str_hashcode_bkdr(const void *input);      /* bkdr�㷨. keyΪ�ַ���ʱ, ���Ƽ� */
size_t cs_linkedhashmap_calc_str_hashcode_elf(const void *input);       /* ELF�㷨. keyΪ�ַ���ʱ, ���Ƽ� */
size_t cs_linkedhashmap_calc_pointer_hashcode_fnv(const void *input);   /* FNV�㷨 */

/* �ṩ����Ĭ�ϵ�key�ԱȺ��� */
unsigned int cs_linkedhashmap_is_value_equals(const void* lhs, const void* rhs);
unsigned int cs_linkedhashmap_is_str_equals(const void* lhs, const void* rhs);

#ifdef __cplusplus
}
#endif

#endif
