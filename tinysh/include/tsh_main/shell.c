#include "tsh_main/tsh_config.h"
#include "tinysh/tinysh.h"
#include "tinysh/cli_cmd.h"

#if (TSH_SUPPORT_RTOS == 1)
#include "FreeRTOS.h"
#include "task.h"
#endif


void shell_version(CLI_ARG* arg)
{
    tsh_print("tsh version:%s\r\n", TSH_VERSION);
}

void shell_sysinfo(CLI_ARG* arg)
{
#if (TSH_SUPPORT_RTOS == 1)
    tsh_print("Free heap size  : %u B\r\n", xPortGetMinimumEverFreeHeapSize());
#else

#endif
}

void shell_clear(CLI_ARG* arg)
{
    tsh_print("\033[2J");
    tsh_print("\x1b[H");
}

void shell_demo(CLI_ARG* arg)
{
    unsigned char i = 0;
    for (i = 0; i < arg->argc; i++)
    {
      tsh_print("argv[%u]:%s\r\n", i, arg->argv[i]);
    }
}

void shell_help(CLI_ARG* arg)
{
    tsh_cmd_iterator();
}

void shell_add_cmd(void)
{
    tsh_cmd_add("help",     shell_help,    "print all cmd");
    tsh_cmd_add("version",  shell_version, "show tsh version");
    tsh_cmd_add("sys_info", shell_sysinfo, "show sysinfo");
    tsh_cmd_add("clear",    shell_clear,   "clean screen");
    tsh_cmd_add("demo",     shell_demo,    "tinysh cli demo");
}

