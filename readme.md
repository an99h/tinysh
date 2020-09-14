# Tinysh

> 极小的shell，简称tsh，适用于嵌入式MCU添加CLI

### 使用步骤

1. 克隆`Tinysh`
3. 配置工程头文件索引 `xxx/Tinysh/include`  `xxx/csl/include`
3. 添加3rdparty cls库（example里有）
4. 在自己工程添加初始化函数 `tsh_init("uname")`
5. 有RTOS环境需要创建`tsh_uart_task()`任务
6. 无RTOS裸机环境在主循环里添加`tsh_cli_msg_procedure()`
7. 修改`tsh_config.h`
8. 在`shell.c`添加自己的命令
9. 添加几个函数定义

### tsh_config.h介绍

```c
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

/* Tinysh 默认是否开启密码 */
#define TSH_PASSWORD_ON     1

/* Tinysh打印函数 根据自己平台修改*/
#define tsh_print        printf

/* 根据自己平台UART结构修改UART_DEV */
typedef USART_TypeDef uart_dev_t;
```

### shell.c使用方法

```c
void shell_demo(CLI_ARG* arg)
{
    unsigned char i = 0;
    for (i = 0; i < arg->argc; i++)
    {
      tsh_print("argv[%u]:%s\r\n", i, arg->argv[i]);
    }
}
int var1 = 0;
int var2 = 1;
void shell_add_cmd(void)
{
    TSH_CMD_ADD_CLI("version",  shell_version, "show tsh version");
    TSH_CMD_ADD_CLI("sys_info", shell_sysinfo, "show sysinfo");
    TSH_CMD_ADD_CLI("clear",    shell_clear,   "clean screen");
    TSH_CMD_ADD_CLI("demo",     shell_demo,    "tinysh cli demo");
    TSH_CMD_ADD_VAR(var1);
    TSH_CMD_ADD_VAR(var2);
}
```

在函数shell_add_cmd()里添加自己的命令

格式为：

- TSH_CMD_ADD_CLI【命令，处理函数，命令描述】
- TSH_CMD_ADD_VAR【全局变量名】

### 在自己工程里完成以下函数定义

```c
void tsh_uart_init(void);   /* tsh 用到uart初始化 */
void tsh_disable_interrupt(void);  /* 关闭全局中断 */
void tsh_enable_interrupt(void);   /* 开启全局中断 */ 
/* uart 读一个字节  */
bool tsh_uart_recv_byte(uart_dev_t* dev, char* ch);                     
/* uart 发一个字节  */
void tsh_uart_send_byte(uart_dev_t* dev, char ch);                      
/* uart 发送多个字节*/
void tsh_uart_send_data(uart_dev_t* dev, char* buf, unsigned char len); 
```

### 配置UART RX中断

在uart中断处理函数中先判断是否为RX中断，如果是再调用`void tsh_uart_irq(uart_dev_t* uart);`

### Example介绍

- Platform：MDK
- MCU：STM32F107
- example：
  - stm32-with-rtos
  - stm32-no-rtos

**注意：编译对应的example需要修改对应的宏 TSH_SUPPORT_RTOS**

### 示例效果

```shell
stm32:)login!
stm32:)login!
stm32:)****
login error!!!
stm32:)****
login success!
stm32:)
stm32:)help
-c- help           ---  print all cmd
-c- version        ---  show tsh version
-c- sys_info       ---  show sysinfo
-c- clear          ---  clean screen
-c- demo           ---  tinysh cli demo
-c- g_variable     ---  show all global variable
-v- var1           ---  get/set var1
-v- var2           ---  get/set var2
stm32:)
stm32:)version
tsh version:1.1.0
stm32:)
stm32:)demo 123 456 789
argv[0]:demo
argv[1]:123
argv[2]:456
argv[3]:789
stm32:)
stm32:)hello world
tsh: command not found: hello
stm32:)
stm32:)var1  
var1 = 0
stm32:)var1 = 100
stm32:)var1
var1 = 100
```
