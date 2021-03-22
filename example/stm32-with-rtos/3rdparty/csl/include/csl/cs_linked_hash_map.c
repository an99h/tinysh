#include "cs_linked_hash_map.h"

/* 私有函数 */
static void _cs_linkedhashmap_free_buckets(cs_linkedhashmap_t* linkedhashmap, cs_linkedhashmap_bucket_t* head);
static void _cs_linkedhashmap_list_insert(cs_linkedhashmap_t* linkedhashmap, cs_linkedhashmap_bucket_t* prev, cs_linkedhashmap_bucket_t* node);
static void _cs_linkedhashmap_list_push_back(cs_linkedhashmap_t* linkedhashmap, cs_linkedhashmap_bucket_t* node);
static int  _cs_linkedhashmap_calc_hashcode(cs_linkedhashmap_t* linkedhashmap, const void* key);
static void _cs_linkedhashmap_list_erase(cs_linkedhashmap_t *linkedhashmap, cs_linkedhashmap_bucket_t *node);
static cs_linkedhashmap_bucket_t* _cs_linkedhashmap_find_bucket(cs_linkedhashmap_t* context, cs_linkedhashmap_bucket_t* head, const void* key);

/**
 * @brief hash值算法
 * @param [in] input hash key
 * @return hash值
 */
size_t cs_linkedhashmap_calc_int_hashcode(const void* input)
{
    return (size_t)input;
}

static void _cs_linkedhashmap_free_buckets(cs_linkedhashmap_t* linkedhashmap, cs_linkedhashmap_bucket_t* head)
{
    cs_linkedhashmap_bucket_t* next = NULL;
    assert(linkedhashmap);
    while (head)
    {
        next = head->_hash_next;
        cs_free(head);
        head = next;
    }
}

static void _cs_linkedhashmap_list_insert(cs_linkedhashmap_t* linkedhashmap, cs_linkedhashmap_bucket_t* prev, cs_linkedhashmap_bucket_t* node)
{
    cs_linkedhashmap_bucket_t *next;

    assert(linkedhashmap);
    if (prev == NULL)
    {
        next = linkedhashmap->_lst_head;
        linkedhashmap->_lst_head = node;
    }
    else
    {
        next = prev->_lst_next;
        prev->_lst_next = node;
    }

    if (next == NULL)
    {
        linkedhashmap->_lst_tail = node;
    }
    else
    {
        next->_lst_prev = node;
    }

    node->_lst_next = next;
    node->_lst_prev = prev;
}

static void _cs_linkedhashmap_list_push_back(cs_linkedhashmap_t* linkedhashmap, cs_linkedhashmap_bucket_t* node)
{
    _cs_linkedhashmap_list_insert(linkedhashmap, linkedhashmap->_lst_tail, node);
}

static int _cs_linkedhashmap_calc_hashcode(cs_linkedhashmap_t* linkedhashmap, const void* key)
{
    assert(linkedhashmap);
    return linkedhashmap->_pfn_calc_hashcode(key) % linkedhashmap->_array_count;
}

static cs_linkedhashmap_bucket_t* _cs_linkedhashmap_find_bucket(cs_linkedhashmap_t* linkedhashmap, cs_linkedhashmap_bucket_t* head, const void* key)
{
    assert(linkedhashmap);
    while (head)
    {
        if (linkedhashmap->_pfn_is_key_equal(key, head->key))
        {
            return head;
        }
        head = head->_hash_next;
    }
    return NULL;
}

static void _cs_linkedhashmap_list_erase(cs_linkedhashmap_t *linkedhashmap, cs_linkedhashmap_bucket_t *node)
{
    if (node->_lst_prev == NULL)
    {
        linkedhashmap->_lst_head = node->_lst_next;
    }
    else
    {
        node->_lst_prev->_lst_next = node->_lst_next;
    }

    if (node->_lst_next == NULL)
    {
        linkedhashmap->_lst_tail = node->_lst_prev;
    }
    else
    {
        node->_lst_next->_lst_prev = node->_lst_prev;
    }
}

/**
 * @brief Bkdr hash算法
 * @param [in] input key
 * @return hash值
 * @attention key为字符串时, 首推荐
 */
size_t cs_linkedhashmap_calc_str_hashcode_bkdr(const void *input)
{
    const char* buf = (const char*)input;
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    while (*buf)
    {
        hash = hash * seed + (*buf++);
    }
    return hash & 0x7fffffff;
}

/**
 * @brief Elf hash算法
 * @param [in] input hash key
 * @return hash值
 * @attention key为字符串时, 次推荐
 */
size_t cs_linkedhashmap_calc_str_hashcode_elf(const void *input)
{
    const char* buf = (const char*)input;
    unsigned int hash = 0;
    unsigned int x = 0;
    while (*buf)
    {
        hash = (hash << 4) + (*buf++);
        if ((x = hash & 0xf0000000l) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
    return hash & 0x7fffffff;
}

/**
 * @brief Fnv hash算法
 * @param [in] input hash key
 * @return hash值
 */
size_t cs_linkedhashmap_calc_pointer_hashcode_fnv(const void *input)
{
    size_t n = (size_t)input;
    char* chArr = (char*)&n;
    size_t hash = (size_t)2166136261UL;
    int i = 0;
    for (; i < sizeof(size_t); i++)
    {
        hash = (hash * 16777619) ^ chArr[i];
    }

    return hash;
}

/**
 * @brief key对比函数
 * @param [in] lhs hash key值
 * @param [in] rhs hash key值
 * @return 相同返回1
 */
unsigned int cs_linkedhashmap_is_value_equals(const void* lhs, const void* rhs)
{
    return (lhs == rhs ? 1 : 0);
}

/**
 * @brief key对比函数
 * @param [in] lhs hash key值
 * @param [in] rhs hash key值
 * @return 相同返回1
 */
unsigned int cs_linkedhashmap_is_str_equals(const void* lhs, const void* rhs)
{
    if (lhs && rhs)
    {
        return 0 == strcmp((const char*)lhs, (const char*)rhs) ? 1 : 0;
    }
    return cs_linkedhashmap_is_value_equals(lhs, rhs);
}

/**
 * @brief hash表初始化
 * @param [in] ctx 上下文
 * @param [in] array_count hash数组大小
 * @return 成功返回0
 */
int cs_linkedhashmap_init(cs_linkedhashmap_t* linkedhashmap, size_t array_count)
{
    if (!linkedhashmap || 0 == array_count)
    {
        return -1;
    }

    memset(linkedhashmap, 0, sizeof(cs_linkedhashmap_t));

    linkedhashmap->_buckets = (cs_linkedhashmap_bucket_t*)cs_malloc(sizeof(cs_linkedhashmap_bucket_t) * array_count);
    if (!linkedhashmap->_buckets)
    {
        return -2;
    }

    memset(linkedhashmap->_buckets, 0, sizeof(cs_linkedhashmap_bucket_t) * array_count);
    linkedhashmap->_array_count = array_count;
    linkedhashmap->_elem_count = 0;
    linkedhashmap->_lst_head = NULL;
    linkedhashmap->_lst_tail = NULL;
    linkedhashmap->_pfn_calc_hashcode = cs_linkedhashmap_calc_int_hashcode;
    linkedhashmap->_pfn_is_key_equal = cs_linkedhashmap_is_value_equals;
    return 0;
}

/**
 * @brief hash表反初始化
 * @param [in] ctx 上下文
 */
void cs_linkedhashmap_uninit(cs_linkedhashmap_t* linkedhashmap)
{
    int i = 0;

    if (!linkedhashmap)
    {
        return;
    }

    for (i = 0; i < linkedhashmap->_array_count; i++)
    {
        _cs_linkedhashmap_free_buckets(linkedhashmap, linkedhashmap->_buckets[i]._hash_next);
    }
    cs_free(linkedhashmap->_buckets);
    linkedhashmap->_buckets = NULL;
    linkedhashmap->_array_count = 0;
    linkedhashmap->_elem_count = 0;
    linkedhashmap->_pfn_calc_hashcode = 0;
    linkedhashmap->_lst_head = NULL;
    linkedhashmap->_lst_tail = NULL;
}

/**
 * @brief hash表插入元素
 * @param [in] ctx 上下文
 * @param [in] key hash key
 * @param [in] value 元素
 * @return 成功返回0
 */
int cs_linkedhashmap_insert(cs_linkedhashmap_t* linkedhashmap, void* key, void* value)
{
    int ret = 0;
    cs_linkedhashmap_bucket_t* new_node = NULL;
    cs_linkedhashmap_bucket_t* bucket = NULL;

    if (!linkedhashmap)
    {
        ret = -1;
        goto exit0;
    }

    bucket = &linkedhashmap->_buckets[_cs_linkedhashmap_calc_hashcode(linkedhashmap, key)];
    if (!bucket->_is_used)
    {
        bucket->key = key;
        bucket->val = value;
        bucket->_is_used = 1;
        _cs_linkedhashmap_list_push_back(linkedhashmap, bucket);
        linkedhashmap->_elem_count++;
        goto exit0;
    }

    /* 相等直接覆盖 */
    if (linkedhashmap->_pfn_is_key_equal(bucket->key, key))
    {
        bucket->val = value;
        goto exit0;
    }

    /* 遍历链表, 若有相等, 则覆盖. */
    while (NULL != bucket->_hash_next)
    {
        bucket = bucket->_hash_next;
        if (linkedhashmap->_pfn_is_key_equal(bucket->key, key))
        {
            bucket->val = value;
            goto exit0;
        }
    }

    /* 新桶*/
    new_node = cs_malloc(sizeof(cs_linkedhashmap_bucket_t));
    if (!new_node)
    {
        ret = -2;
        goto exit0;
    }
    new_node->key = key;
    new_node->val = value;
    new_node->_hash_next = NULL;
    new_node->_is_used = 1;
    bucket->_hash_next = new_node;
    _cs_linkedhashmap_list_push_back(linkedhashmap, new_node);
    linkedhashmap->_elem_count++;

exit0:
    return ret;
}

/**
 * @brief 判断该key是否存在hash表里
 * @param [in] ctx 上下文
 * @param [in] key hash key
 * @return bool
 */
bool cs_linkedhashmap_is_exist(cs_linkedhashmap_t* linkedhashmap, const void* key)
{
    if (linkedhashmap)
    {
        cs_linkedhashmap_bucket_t* bucket = &linkedhashmap->_buckets[_cs_linkedhashmap_calc_hashcode(linkedhashmap, key)];
        cs_linkedhashmap_bucket_t* f = _cs_linkedhashmap_find_bucket(linkedhashmap, bucket, key);
        if (f)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief 通过key获取存放在hash表里的值
 * @param [in] ctx 上下文
 * @param [in] key hash key
 * @return 存放在hash表里的值
 */
void* cs_linkedhashmap_get(cs_linkedhashmap_t* linkedhashmap, const void* key)
{
    if (linkedhashmap)
    {
        cs_linkedhashmap_bucket_t* bucket = &linkedhashmap->_buckets[_cs_linkedhashmap_calc_hashcode(linkedhashmap, key)];
        cs_linkedhashmap_bucket_t* f = _cs_linkedhashmap_find_bucket(linkedhashmap, bucket, key);
        if (f)
        {
            return f->val;
        }
    }

    return NULL;
}

/**
 * @brief 通过key删除存放在hash表里的值
 * @param [in] ctx 上下文
 * @param [in] key hash key
 */
void cs_linkedhashmap_erase(cs_linkedhashmap_t* linkedhashmap, void* key)
{
    cs_linkedhashmap_bucket_t *bucket = NULL;

    if (!linkedhashmap)
    {
        goto exit0;
    }

    bucket = &linkedhashmap->_buckets[_cs_linkedhashmap_calc_hashcode(linkedhashmap, key)];
    if (linkedhashmap->_pfn_is_key_equal(key, bucket->key))
    {
        if (bucket->_hash_next)
        {
            cs_linkedhashmap_bucket_t* tmp = bucket->_hash_next;
            bucket->key = tmp->key;
            bucket->val = tmp->val;
            bucket->_hash_next = tmp->_hash_next;
            _cs_linkedhashmap_list_erase(linkedhashmap, tmp);

            cs_free(tmp);
            linkedhashmap->_elem_count--;
            goto exit0;
        }
        else
        {
            _cs_linkedhashmap_list_erase(linkedhashmap, bucket);
            bucket->key = NULL;
            linkedhashmap->_elem_count--;
            goto exit0;
        }
    }
    else
    {
        cs_linkedhashmap_bucket_t* next = bucket->_hash_next;
        while (next)
        {
            if (linkedhashmap->_pfn_is_key_equal(key, next->key))
            {
                bucket->_hash_next = next->_hash_next;
                _cs_linkedhashmap_list_erase(linkedhashmap, next);
                cs_free(next);
                linkedhashmap->_elem_count--;
                goto exit0;
            }
            bucket = next;
            next = bucket->_hash_next;
        }
    }

exit0:
    return;
}
