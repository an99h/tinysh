#include "tsh_main/tsh_config.h"
#include "tinysh/tinysh.h"
#include "tinysh/cli_cmd.h"

#if (TSH_SUPPORT_RTOS == 1)
#include "FreeRTOS.h"
#include "task.h"
#endif


static void shell_version(CLI_ARG* arg)
{
    tsh_print("tsh version:%s\r\n", TSH_VERSION);
}

static void shell_sysinfo(CLI_ARG* arg)
{
#if (TSH_SUPPORT_RTOS == 1)
    tsh_print("Free heap size  : %u B\r\n", xPortGetMinimumEverFreeHeapSize());
#else

#endif
}

static void shell_clear(CLI_ARG* arg)
{
    tsh_print("\033[2J");
    tsh_print("\x1b[H");
}

static void shell_demo(CLI_ARG* arg)
{
    unsigned char i = 0;
    for (i = 0; i < arg->argc; i++)
    {
      tsh_print("argv[%u]:%s\r\n", i, arg->argv[i]);
    }
}

static void shell_help(CLI_ARG* arg)
{
    tsh_cmd_iterator(TSH_TYPE_ALL);
}

static void shell_g_variable(CLI_ARG* arg)
{
    tsh_cmd_iterator(TSH_TYPE_VAR);
}

int var1 = 0;
int var2 = 1;

void shell_add_cmd(void)
{
    TSH_CMD_ADD_CLI("help",       shell_help,       "print all cmd");
    TSH_CMD_ADD_CLI("version",    shell_version,    "show tsh version");
    TSH_CMD_ADD_CLI("sys_info",   shell_sysinfo,    "show sysinfo");
    TSH_CMD_ADD_CLI("clear",      shell_clear,      "clean screen");
    TSH_CMD_ADD_CLI("demo",       shell_demo,       "tinysh cli demo");
    TSH_CMD_ADD_CLI("g_variable", shell_g_variable, "show all global variable");
    TSH_CMD_ADD_VAR(var1);
    TSH_CMD_ADD_VAR(var2);
}

