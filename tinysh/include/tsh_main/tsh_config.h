/* �����Լ���Ŀuartͷ�ļ�*/
#include "usart.h"

/* �Ƿ�֧��RTOS    */
#define TSH_SUPPORT_RTOS  1

/* �����в�������   */
#define TSH_MSG_ARGC      5

/* ��Ϣ���и���    */
#define TSH_MSG_COUNT     2

/* ÿ����Ϣ����    */
#define TSH_MSG_SIZE      32

/* ���֧��������� */
#define TSH_MAX_CMD_COUNT 20           

/* ���֧����ʷ��¼���� */
#define TSH_HISTORY_COUNT 20

/* Tinysh����  logout�˳���¼  */
#define TSH_PASSWORD     "root"

/* Tinysh��ӡ���� �����Լ�ƽ̨�޸�*/
#define tsh_print        printf

/* �����Լ�ƽ̨UART�ṹ�޸�UART_DEV */
typedef USART_TypeDef uart_dev_t;

