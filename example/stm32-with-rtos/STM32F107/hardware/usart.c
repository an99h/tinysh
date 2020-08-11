#include <stdio.h>
#include "usart.h"
#include "tinysh/tinysh.h"

#define PRINTF             UART1
#define LOG_TAG            "usart"
#define USART_RECV_MAX_LEN MSG_MAX_SIZE

extern int g_login;

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)

//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

//重定义fputc函数 
int fputc(int ch, FILE* f)
{
    while ((PRINTF->SR & 0X40) == 0);//循环发送,直到发送完毕
    PRINTF->DR = (u8)ch;
    return ch;
}
#endif 

/*使用microLib的方法*/
#if 0
int fputc(int ch, FILE * f)
{
    USART_SendData(USART1, (uint8_t)ch);

    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}

    return ch;
}
int GetKey(void) {

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
#endif


void uart_init(USART_TypeDef* UARTx, u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    if (UARTx == USART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);

        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    }
    else if (UARTx == USART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);

        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    }
    else if (UARTx == UART4)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, DISABLE);

        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    }
    else if (UARTx == UART5)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, DISABLE);

        NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    }

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UARTx, &USART_InitStructure);
    USART_ITConfig(UARTx, USART_IT_RXNE, ENABLE);
    USART_Cmd(UARTx, ENABLE);
}

void uart_send_data(USART_TypeDef* UARTx, u8* buf, u16 len)
{
    u8 i = 0;
    for (; i < len; i++)
    {
        while (USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET);
        USART_SendData(UARTx, buf[i]);
    }
    while (USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET);
}

void UART_Send_Byte(USART_TypeDef* UARTx, u8 ch)
{
    while (USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET);
    USART_SendData(UARTx, ch);
    while (USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET);
}

int uart_read_byte(USART_TypeDef* UARTx, u8* ch)
{
    if (USART_GetFlagStatus(UARTx, USART_FLAG_RXNE) != RESET)
    {
        *ch = USART_ReceiveData(UARTx);
        return 0;
    }
    else
    {
        return -1;
    }
}

void tsh_uart_init(void)
{
    uart_init(UART1, 115200);
}

void tsh_disable_interrupt(void)
{
    INTX_DISABLE();
}
void tsh_enable_interrupt(void)
{
    INTX_ENABLE();
}
int  tsh_uart_recv_byte(uart_dev_t* dev, char* ch)
{
    return uart_read_byte(dev, (u8*)ch);
}
void tsh_uart_send_byte(uart_dev_t* dev, char ch)
{
    UART_Send_Byte(dev, (u8)ch);
}
void tsh_uart_send_data(uart_dev_t* dev, char* buf, unsigned char len)
{
    uart_send_data(dev, (u8*)buf, len);
}

void USART1_IRQHandler(void)
{
    tsh_uart_irq(UART1);
}
#if 0

//串口1中断服务程序
void USART1_IRQHandler(void)
{
    static cs_memwriter_t mem;
    static char* cache = NULL;
    u8 ch = 0;
    int cur_len = 0;
    u8 stop_flag = 0;
    static int key_direction = 0;

    if (USART_GetITStatus(UART1, USART_IT_RXNE) == RESET)
    {
        return;
    }

    if (!cache)
    {
        cache = (char*)cs_object_pool_obtain(msg_pool);
        cs_memwriter_attach(&mem, cache, USART_RECV_MAX_LEN);
    }

    ch = USART_ReceiveData(UART1);

    if (ch == 0x1b || key_direction != 0) // 方向键
    {
        u8 i = 0;
        u8 old_len = 0;
        char cmd[32] = { 0 };
        key_direction++;
        if (key_direction == 3)
        {
            key_direction = 0;
            if (get_shell_history(ch, cmd))
            {
                old_len = cs_memwriter_get_cursor(&mem);
                for (i = 0; i < old_len; i++)
                {
                    uart_send_data(UART1, "\b \b", 3);
                }
                cs_memwriter_seek(&mem, 0);
                cs_memwriter_push_back_binary(&mem, &cmd, strlen(cmd));
                uart_send_data(UART1, (u8*)cmd, strlen(cmd));
            }
        }
        return;
    }

    if (ch != 0x08)  // !删除键
    {
        if (g_login)
        {
            USART_SendData(UART1, ch);
        }
        else
        {
            if (ch != 0xa && ch != 0xd)
            {
                USART_SendData(UART1, '*');
            }
        }
        cs_memwriter_push_back_binary(&mem, &ch, sizeof(u8));
    }
    else
    {
        cur_len = cs_memwriter_get_cursor(&mem);
        if (cur_len - 1 >= 0)
        {
            uart_send_data(UART1, "\b \b", 3);
            cs_memwriter_seek(&mem, cur_len - 1);
            cache[cur_len - 1] = 0;
        }
        return;
    }

    cur_len = cs_memwriter_get_cursor(&mem);
    if (cur_len > USART_RECV_MAX_LEN)
    {
        memset(cache, 0, USART_RECV_MAX_LEN);
        cs_memwriter_seek(&mem, 0);
        return;
    }

    if (ch == 0x0a)
    {
        if (cache[cur_len - 2] == 0xd)
        {
            //去除结束符 \r\n
            cache[cur_len - 1] = 0;
            cache[cur_len - 2] = 0;
            cur_len -= 2;
            stop_flag = 1;
        }
    }

    if (stop_flag)
    {
        _UART_Send_Queue(UART1_MSG, NULL, cur_len, cache);
        cache = NULL;
    }
}

#endif
