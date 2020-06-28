/* 包含自己项目uart头文件*/
#include "usart.h"

/* 是否支持RTOS    */
#define TSH_SUPPORT_RTOS  1

/* 命令行参数个数   */
#define TSH_MSG_ARGC      5

/* 消息队列个数    */
#define TSH_MSG_COUNT     2

/* 每条消息长度    */
#define TSH_MSG_SIZE      32

/* 最大支持命令个数 */
#define TSH_MAX_CMD_COUNT 20           

/* 最大支持历史记录个数 */
#define TSH_HISTORY_COUNT 20

/* Tinysh密码  logout退出登录  */
#define TSH_PASSWORD     "root"

/* Tinysh打印函数 根据自己平台修改*/
#define tsh_print        printf

/* 根据自己平台UART结构修改UART_DEV */
typedef USART_TypeDef uart_dev_t;

