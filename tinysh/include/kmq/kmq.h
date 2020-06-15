#ifndef KMQ_H_3B6D559A_F7D5_40FF_A4E5_762DE3B4A123
#define KMQ_H_3B6D559A_F7D5_40FF_A4E5_762DE3B4A123

#include "tsh_main/tsh_config.h"
#if (TSH_SUPPORT_RTOS == 1)
#include "FreeRTOS.h"
#include "queue.h"

//typedef char bool;

#define bool int

typedef struct kmq_tag
{
    QueueHandle_t _qid;
} kmq_t;


#define KMQ_WAIT_FOREVER portMAX_DELAY /*!< ��Զ�ȴ� */

enum
{
    KMQ_MSG_EMPTY = 0,        /*!< ����Ϣ */
    KMQ_MSG_QUIT,             /*!< �˳���Ϣ */
    KMQ_MSG_TIMER,            /*!< ��ʱ����Ϣ */
    KMQ_MSG_USER = 100,       /*!< ��Χ[0,100)ΪKMQ�ڲ�Ԥ��, ����Ϊ�û��Զ�����Ϣ */
};

/**
 * kmq_value���ڼ���C����ԭ���������͵Ĵ洢����
 */
typedef union
{
    char          char_value;
    unsigned char uchar_value;
    int           int_value;
    unsigned int  uint_value;
    long          long_value;
    unsigned long ulong_value;
    void* pointer_value;
    double        double_value;
    float         float_value;
} kmq_value_t;

struct kmq_msg_tag;

/**
 * @brief ��Ϣ�ص��ӿ�
 *   ��Ϣѭ�������Ȼص��˽ӿڽ�����Ϣ����, ����˽ӿڲ�����, ���ٽ�����Ϣ���̺�������
 * @return �Ƿ������������Ϣ, �������false, �����Ϣ�ᱻ�ɷ�����Ϣ���̺���.
 *  @retval true ������������Ϣ
 *  @retval false ��������Ϣ��, ��������Ϣ�ɷ�����Ϣ���̺�������
 * @see on_kmq_msg_procedure_t
 */
typedef bool(*on_kmq_msg_callback_t)(const kmq_t* q, const struct kmq_msg_tag* msg, void* param);

/**
 * @brief ��Ϣ�ṹ��
 *   �洢������Ϣ��Я������Ϣ
 */
typedef struct kmq_msg_tag
{
    int id;                          /*!< ��ϢID, ���ڱ�ʶ��ͬ����Ϣ */
    int isr;                         /*!< �Ƿ����ж���ִ�� */
    int len;                         /*!< �������� */
    kmq_value_t arg;                 /*!< ���� */
    on_kmq_msg_callback_t callback;  /*!< ��Ϣ�ص��ӿ� */
} kmq_msg_t;

/**
 * @brief ��Ϣ���̺���
 *   ����Ϣ����������Ϣʱ, ��Ϣѭ����ص��˺���������Ϣ�ɷ�
 * @param [in] q ��Ϣ�����Ķ���
 * @param [in] msg ��Ϣ�ṹ��
 * @param [in] param �ص�����, ��������ɵ���kmq_run��ʱָ��
 */
typedef void(*on_kmq_msg_procedure)(const kmq_t* q, const struct kmq_msg_tag* msg, void* param);

/**
 * @brief ��ʼ��KMQ����
 * @param [in] q kmq����
 * @param [in] count ���д�С, ��ʾ��������������ɶ�������Ϣ
 * @return �ɹ�����0
 */
BaseType_t kmq_init(kmq_t* q, int count);

/**
 * @brief ����ʼ��KMQ
 * @param [in] q kmq����
 */
void kmq_uninit(kmq_t* q);

/**
 * @brief ����з�����Ϣ
 * @param [in] q kmq����
 * @param [in] msg ��Ϣ����
 * @param [in] timeout ��ʱ, ��λ:����
 * @return ���жϵ��óɹ�����0 
           ������жϵ��÷���ֵ�� �����л���־xHigherPriorityTaskWoken
 */
BaseType_t kmq_send_msg(kmq_t* q, kmq_msg_t* msg, unsigned int timeout);

/**
 * @brief ���ն�����Ϣ
 * @param [in] q kmq����
 * @param [in] msg ���ڴ洢Ҫ������Ϣ���׵�ַ
 * @param [in] timeout ��ʱ, ��λ:����
 * @return �ɹ�����0
 */
BaseType_t kmq_recv_msg(kmq_t* q, kmq_msg_t* msg, unsigned int timeout);

/**
 * @brief ����з����˳���Ϣ
 * @param [in] q kmq����
 * @param [in] isr ��ǰ�Ƿ����жϺ�����ִ��
 * @param [in] timeout ��ʱ, ��λ:����
 * @return �ɹ�����0
 */
BaseType_t kmq_quit(kmq_t* q, int isr, unsigned int timeout);

/**
 * @brief ������Ϣѭ��
 *
 *   ���ú�, �߳̽���������״̬. �ȴ�����Ϣ���͹����������ɷ�����Ϣ���̺���ȥ����
 *   �ڲ���һ����ѭ��, ������Ϣ����, ����kmq_quit����з����˳���Ϣ��, ��ʹ�����ѭ������.
 *
 * @param [in] q kmq����
 * @param [in] procedure ��Ϣ���̺���
 * @param [in] param ���Ӳ���
 * @return �ɹ�����0
 */
BaseType_t kmq_run_loop(kmq_t* q, on_kmq_msg_procedure procedure, void* param);

void kmq_send_q(kmq_t* q, int msg_id, int isr, char* buf, int len, on_kmq_msg_callback_t callback);

#endif
#endif
