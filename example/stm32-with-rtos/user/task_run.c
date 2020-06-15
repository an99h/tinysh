#include "task_run.h"
#include "main.h"
#include "tinysh/tinysh.h"

#define LOG_TAG "task_run"

#define START_TASK_PRIO        1         // �������ȼ� 0���
#define START_STK_SIZE         128       // �����ջ��С * 4�ֽ�
static TaskHandle_t g_start_task_handle; // ������

#define UART_TASK_PRIO        7
#define UART_STK_SIZE         768
static TaskHandle_t g_uart_task_handle;

static void start_task(void* param);

void task_run(void)
{
    xTaskCreate((TaskFunction_t)start_task,     //������
        (const char*)"start_task",              //��������
        (uint16_t)START_STK_SIZE,               //�����ջ��С
        (void*)NULL,                            //���ݸ��������Ĳ���
        (UBaseType_t)START_TASK_PRIO,           //�������ȼ�
        (TaskHandle_t*)&g_start_task_handle);   //������
    vTaskStartScheduler(); //�����������
}


//��ʼ����������
static void start_task(void* param)
{
    taskENTER_CRITICAL();           //�����ٽ���
    xTaskCreate((TaskFunction_t)tsh_uart_task,
        (const char*)"tsh_task",
        (uint16_t)UART_STK_SIZE,
        (void*)NULL,
        (UBaseType_t)UART_TASK_PRIO,
        (TaskHandle_t*)&g_uart_task_handle);
    vTaskDelete(g_start_task_handle);
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

