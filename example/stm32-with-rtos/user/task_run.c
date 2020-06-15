#include "task_run.h"
#include "main.h"
#include "tinysh/tinysh.h"

#define LOG_TAG "task_run"

#define START_TASK_PRIO        1         // 任务优先级 0最低
#define START_STK_SIZE         128       // 任务堆栈大小 * 4字节
static TaskHandle_t g_start_task_handle; // 任务句柄

#define UART_TASK_PRIO        7
#define UART_STK_SIZE         768
static TaskHandle_t g_uart_task_handle;

static void start_task(void* param);

void task_run(void)
{
    xTaskCreate((TaskFunction_t)start_task,     //任务函数
        (const char*)"start_task",              //任务名称
        (uint16_t)START_STK_SIZE,               //任务堆栈大小
        (void*)NULL,                            //传递给任务函数的参数
        (UBaseType_t)START_TASK_PRIO,           //任务优先级
        (TaskHandle_t*)&g_start_task_handle);   //任务句柄
    vTaskStartScheduler(); //开启任务调度
}


//开始任务任务函数
static void start_task(void* param)
{
    taskENTER_CRITICAL();           //进入临界区
    xTaskCreate((TaskFunction_t)tsh_uart_task,
        (const char*)"tsh_task",
        (uint16_t)UART_STK_SIZE,
        (void*)NULL,
        (UBaseType_t)UART_TASK_PRIO,
        (TaskHandle_t*)&g_uart_task_handle);
    vTaskDelete(g_start_task_handle);
    taskEXIT_CRITICAL();            //退出临界区
}

