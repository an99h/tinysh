#ifndef __USART_H
#define __USART_H
#include <string.h>
#include <stdio.h>
#include "sys.h"

#define UART1 USART1
#define UART2 USART2
#define UART3 USART3

//初始化 USART1，USART2，UART4，UART5
void uart_init(USART_TypeDef* UARTx, u32 bound);

//发送 USART1，USART2，UART4，UART5 数据
void uart_send_data(USART_TypeDef* UARTx, u8* buf, u16 len);

int uart_read_byte(USART_TypeDef* UARTx, u8* ch);
#endif


