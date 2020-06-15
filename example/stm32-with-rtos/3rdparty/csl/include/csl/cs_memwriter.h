#ifndef CS_MEM_WRITER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_MEM_WRITER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    char* _data;    /*!< data指针 */
    int   _size;    /*!< 记录data的大小 */
    int   _cur_idx; /*!< 当前下标 */
} cs_memwriter_t;

/**
 * @brief 附到指定的一块buffer上, 这样就可以通过接口来操作这块buffer了
 * @param [in] mem cs_memwriter句柄
 * @param [in] buffer 指定需要被操作的内存首地址
 * @param [in] bytes 内存大小
 * @return 成功返回0
 */
int cs_memwriter_attach(cs_memwriter_t* mem, void* buffer, const int bytes);

/**
 * @brief 指定当前的读/写位置
 * @param [in] mem cs_memwriter句柄
 * @param [in] index 要指定的读/写位置下标, 从0开始
 * @return 成功返回0
 */
int cs_memwriter_seek(cs_memwriter_t* mem, const int index);

/**
 * @brief 从当前读写位置, 向前或向后跳几个字节
 * @param [in] mem cs_memwriter句柄
 * @param [in] step 要跳动的步数, 即字节数, 为负数时向后倒着跳
 * @return 成功返回0
 */
int cs_memwriter_skip(cs_memwriter_t* mem, const int step);

/**
 * @brief 得到当前读/写光标的位置下标
 * @param [in] mem cs_memwriter句柄
 * @return 当前读/写光标的位置下标
 * @attention 光标的位置, 永远处在最后一次读写数据的后面一个字节
 *   1. 如果buffer为空, 光标位置为0
 *   2. 如果写了2个字节数据, 则光标会跳到第3个字节数, 即此时光标下标值等于2
 *   3. 如果buffer被写满, 光标位置等于buffer的大小值
 */
int cs_memwriter_get_cursor(cs_memwriter_t* mem);

/**
 * @brief 写入二进制数据
 * @param [in] mem cs_memwriter句柄
 * @param [in] buffer 要写入的数据指针
 * @param [in] bytes 要写入的数据大小
 * @return 成功返回0
 */
int cs_memwriter_push_back_binary(cs_memwriter_t* mem, const void* buffer, const int bytes);

/**
 * @brief 从前面弹出指定大小的数据
 * @param [in] mem cs_memwriter句柄
 * @param [in] bytes 要弹出的数据的大小
 * @return 被弹出的数据首地址
 * @attention 成功弹出后, 读/写光标会向后移植指定的字节数
 */
void* cs_memwriter_pop_front(cs_memwriter_t* mem, const int bytes);

/**
 * @brief 从后面弹出指定大小的数据
 * @param [in] mem cs_memwriter句柄
 * @param [in] bytes 要弹出的数据的大小
 * @return 被弹出的数据首地址
 * @attention 成功弹出后, 读/写光标会向前移植指定的字节数
 */
void* cs_memwriter_pop_back(cs_memwriter_t* mem, const int bytes);

/**
 * @brief 获取所依附的buffer地址
 * @param [in] mem cs_memwriter句柄
 * @param [out] size 返回buffer的大小, 如果不关心, 可以填NULL
 * @return 依附的buffer地址
 */
void* cs_memwriter_get_buffer(cs_memwriter_t* mem, size_t* size);

/**
 * @def cs_memwriter_push_back(mem, value_type, value)
 * @brief 写入基础类型的数据, 如int, char, long等C语言原生数据类型
 * @param [in] mem cs_memwriter句柄
 * @param [in] value_type 数据的类型, 如整数, 就填int
 * @param [in] value 要写入的数值
 * @par 示例
 * @code
 *   cs_memwriter_push_back(&mem, int, 10);
 *   cs_memwriter_push_back(&mem, double, 3.14);
 *   cs_memwriter_push_back(&mem, unsigned long, 20);
 * @endcode
 */
#define cs_memwriter_push_back(mem, value_type, value)                                  \
    {                                                                                   \
        if ((mem) && ((mem)->_cur_idx + sizeof(value_type) <= (size_t)(mem)->_size))    \
        {                                                                               \
            *(value_type*)(&(mem)->_data[(mem)->_cur_idx]) = value;                     \
            (mem)->_cur_idx += sizeof(value_type);                                      \
        }                                                                               \
    }

#ifdef __cplusplus
}
#endif

#endif
