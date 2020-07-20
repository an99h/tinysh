#ifndef __TINYSH_H__
#define __TINYSH_H__

#include "csl/cs_base_types.h"
#include "tsh_main/tsh_config.h"

#define TSH_VERSION      "1.1.0"          /* Tinysh�汾��  */
#define TSH_MSG_ID_CLI   101

//�û�ʵ��
extern void tsh_uart_init(void);          /* tsh �õ�uart��ʼ�� */
extern void tsh_disable_interrupt(void);  /* �ر�ȫ���ж� */
extern void tsh_enable_interrupt(void);   /* ����ȫ���ж� */ 
extern bool tsh_uart_recv_byte(uart_dev_t* dev, char* ch);                     /* uart ��һ���ֽ�  */
extern void tsh_uart_send_byte(uart_dev_t* dev, char ch);                      /* uart ��һ���ֽ�  */
extern void tsh_uart_send_data(uart_dev_t* dev, char* buf, unsigned char len); /* uart ���Ͷ���ֽ�*/

//�û�����
int  tsh_init(char* uname);           /* ��ʼ��tinysh    uname:tsh user name         */
void tsh_uart_irq(uart_dev_t* uart);  /* ����Ҫ�����UART�ж��е���                   */
#if (TSH_SUPPORT_RTOS == 1)
void tsh_uart_task(void* param);       /* ��RTOS����������� */
#else
void tsh_cli_msg_procedure(void);      /* ��RTOS�� ����ѭ���е��� */
#endif

#endif
