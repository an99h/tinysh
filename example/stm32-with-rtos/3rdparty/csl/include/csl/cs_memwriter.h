#ifndef CS_MEM_WRITER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_MEM_WRITER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    char* _data;    /*!< dataָ�� */
    int   _size;    /*!< ��¼data�Ĵ�С */
    int   _cur_idx; /*!< ��ǰ�±� */
} cs_memwriter_t;

/**
 * @brief ����ָ����һ��buffer��, �����Ϳ���ͨ���ӿ����������buffer��
 * @param [in] mem cs_memwriter���
 * @param [in] buffer ָ����Ҫ���������ڴ��׵�ַ
 * @param [in] bytes �ڴ��С
 * @return �ɹ�����0
 */
int cs_memwriter_attach(cs_memwriter_t* mem, void* buffer, const int bytes);

/**
 * @brief ָ����ǰ�Ķ�/дλ��
 * @param [in] mem cs_memwriter���
 * @param [in] index Ҫָ���Ķ�/дλ���±�, ��0��ʼ
 * @return �ɹ�����0
 */
int cs_memwriter_seek(cs_memwriter_t* mem, const int index);

/**
 * @brief �ӵ�ǰ��дλ��, ��ǰ������������ֽ�
 * @param [in] mem cs_memwriter���
 * @param [in] step Ҫ�����Ĳ���, ���ֽ���, Ϊ����ʱ�������
 * @return �ɹ�����0
 */
int cs_memwriter_skip(cs_memwriter_t* mem, const int step);

/**
 * @brief �õ���ǰ��/д����λ���±�
 * @param [in] mem cs_memwriter���
 * @return ��ǰ��/д����λ���±�
 * @attention ����λ��, ��Զ�������һ�ζ�д���ݵĺ���һ���ֽ�
 *   1. ���bufferΪ��, ���λ��Ϊ0
 *   2. ���д��2���ֽ�����, �����������3���ֽ���, ����ʱ����±�ֵ����2
 *   3. ���buffer��д��, ���λ�õ���buffer�Ĵ�Сֵ
 */
int cs_memwriter_get_cursor(cs_memwriter_t* mem);

/**
 * @brief д�����������
 * @param [in] mem cs_memwriter���
 * @param [in] buffer Ҫд�������ָ��
 * @param [in] bytes Ҫд������ݴ�С
 * @return �ɹ�����0
 */
int cs_memwriter_push_back_binary(cs_memwriter_t* mem, const void* buffer, const int bytes);

/**
 * @brief ��ǰ�浯��ָ����С������
 * @param [in] mem cs_memwriter���
 * @param [in] bytes Ҫ���������ݵĴ�С
 * @return �������������׵�ַ
 * @attention �ɹ�������, ��/д���������ֲָ�����ֽ���
 */
void* cs_memwriter_pop_front(cs_memwriter_t* mem, const int bytes);

/**
 * @brief �Ӻ��浯��ָ����С������
 * @param [in] mem cs_memwriter���
 * @param [in] bytes Ҫ���������ݵĴ�С
 * @return �������������׵�ַ
 * @attention �ɹ�������, ��/д������ǰ��ֲָ�����ֽ���
 */
void* cs_memwriter_pop_back(cs_memwriter_t* mem, const int bytes);

/**
 * @brief ��ȡ��������buffer��ַ
 * @param [in] mem cs_memwriter���
 * @param [out] size ����buffer�Ĵ�С, ���������, ������NULL
 * @return ������buffer��ַ
 */
void* cs_memwriter_get_buffer(cs_memwriter_t* mem, size_t* size);

/**
 * @def cs_memwriter_push_back(mem, value_type, value)
 * @brief д��������͵�����, ��int, char, long��C����ԭ����������
 * @param [in] mem cs_memwriter���
 * @param [in] value_type ���ݵ�����, ������, ����int
 * @param [in] value Ҫд�����ֵ
 * @par ʾ��
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
