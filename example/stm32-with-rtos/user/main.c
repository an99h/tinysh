#include "main.h"
#include "delay.h"
#include "task_run.h"
#include "tinysh/tinysh.h"


int main(void)
{
    sys_init();
    
    tsh_init("stm32");
    
    task_run();
    
    return 0;
}

