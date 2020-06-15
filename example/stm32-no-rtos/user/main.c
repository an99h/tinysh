#include "delay.h"
#include "tinysh/tinysh.h"

static void _init()
{
    sys_init();
    tsh_init("stm32");
}

int main(void)
{
    _init();

    while(1)
    {
        tsh_cli_msg_procedure();
    }
    return 0;
}

