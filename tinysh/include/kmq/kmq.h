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


#define KMQ_WAIT_FOREVER portMAX_DELAY /*!< 永远等待 */

enum
{
    KMQ_MSG_EMPTY = 0,        /*!< 空消息 */
    KMQ_MSG_QUIT,             /*!< 退出消息 */
    KMQ_MSG_TIMER,            /*!< 计时器消息 */
    KMQ_MSG_USER = 100,       /*!< 范围[0,100)为KMQ内部预留, 其余为用户自定义消息 */
};

/**
 * kmq_value用于兼容C语言原生数据类型的存储操作
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
 * @brief 消息回调接口
 *   消息循环会优先回调此接口进行消息处理, 如果此接口不处理, 则再交给消息过程函数处理
 * @return 是否消化掉这个消息, 如果返回false, 则该消息会被派发到消息过程函数.
 *  @retval true 消化掉这条消息
 *  @retval false 处理完消息会, 继续将消息派发给消息过程函数处理
 * @see on_kmq_msg_procedure_t
 */
typedef bool(*on_kmq_msg_callback_t)(const kmq_t* q, const struct kmq_msg_tag* msg, void* param);

/**
 * @brief 消息结构体
 *   存储单个消息所携带的信息
 */
typedef struct kmq_msg_tag
{
    int id;                          /*!< 消息ID, 用于标识不同的消息 */
    int isr;                         /*!< 是否在中断里执行 */
    int len;                         /*!< 参数长度 */
    kmq_value_t arg;                 /*!< 参数 */
    on_kmq_msg_callback_t callback;  /*!< 消息回调接口 */
} kmq_msg_t;

/**
 * @brief 消息过程函数
 *   当消息队列中有消息时, 消息循环会回调此函数进行消息派发
 * @param [in] q 消息所属的队列
 * @param [in] msg 消息结构体
 * @param [in] param 回调参数, 这个参数由调用kmq_run环时指定
 */
typedef void(*on_kmq_msg_procedure)(const kmq_t* q, const struct kmq_msg_tag* msg, void* param);

/**
 * @brief 初始化KMQ队列
 * @param [in] q kmq队列
 * @param [in] count 队列大小, 表示队列中最多能容纳多少条消息
 * @return 成功返回0
 */
BaseType_t kmq_init(kmq_t* q, int count);

/**
 * @brief 反初始化KMQ
 * @param [in] q kmq队列
 */
void kmq_uninit(kmq_t* q);

/**
 * @brief 向队列发送消息
 * @param [in] q kmq队列
 * @param [in] msg 消息内容
 * @param [in] timeout 超时, 单位:毫秒
 * @return 非中断调用成功返回0 
           如果是中断调用返回值是 任务切换标志xHigherPriorityTaskWoken
 */
BaseType_t kmq_send_msg(kmq_t* q, kmq_msg_t* msg, unsigned int timeout);

/**
 * @brief 接收队列消息
 * @param [in] q kmq队列
 * @param [in] msg 用于存储要接收消息的首地址
 * @param [in] timeout 超时, 单位:毫秒
 * @return 成功返回0
 */
BaseType_t kmq_recv_msg(kmq_t* q, kmq_msg_t* msg, unsigned int timeout);

/**
 * @brief 向队列发送退出消息
 * @param [in] q kmq队列
 * @param [in] isr 当前是否在中断函数中执行
 * @param [in] timeout 超时, 单位:毫秒
 * @return 成功返回0
 */
BaseType_t kmq_quit(kmq_t* q, int isr, unsigned int timeout);

/**
 * @brief 进入消息循环
 *
 *   调用后, 线程将进入阻塞状态. 等待有消息发送过来并进行派发到消息过程函数去处理
 *   内部是一个死循环, 监听消息队列, 调用kmq_quit向队列发送退出消息后, 可使这个死循环结束.
 *
 * @param [in] q kmq队列
 * @param [in] procedure 消息过程函数
 * @param [in] param 附加参数
 * @return 成功返回0
 */
BaseType_t kmq_run_loop(kmq_t* q, on_kmq_msg_procedure procedure, void* param);

void kmq_send_q(kmq_t* q, int msg_id, int isr, char* buf, int len, on_kmq_msg_callback_t callback);

#endif
#endif
