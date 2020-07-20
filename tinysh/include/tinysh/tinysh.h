#ifndef __TINYSH_H__
#define __TINYSH_H__

#include "csl/cs_base_types.h"
#include "tsh_main/tsh_config.h"

#define TSH_VERSION      "1.1.0"          /* Tinysh版本号  */
#define TSH_MSG_ID_CLI   101

//用户实现
extern void tsh_uart_init(void);          /* tsh 用到uart初始化 */
extern void tsh_disable_interrupt(void);  /* 关闭全局中断 */
extern void tsh_enable_interrupt(void);   /* 开启全局中断 */ 
extern bool tsh_uart_recv_byte(uart_dev_t* dev, char* ch);                     /* uart 读一个字节  */
extern void tsh_uart_send_byte(uart_dev_t* dev, char ch);                      /* uart 发一个字节  */
extern void tsh_uart_send_data(uart_dev_t* dev, char* buf, unsigned char len); /* uart 发送多个字节*/

//用户调用
int  tsh_init(char* uname);           /* 初始化tinysh    uname:tsh user name         */
void tsh_uart_irq(uart_dev_t* uart);  /* 在需要输出的UART中断中调用                   */
#if (TSH_SUPPORT_RTOS == 1)
void tsh_uart_task(void* param);       /* 在RTOS创建这个任务 */
#else
void tsh_cli_msg_procedure(void);      /* 无RTOS中 在主循环中调用 */
#endif

#endif
