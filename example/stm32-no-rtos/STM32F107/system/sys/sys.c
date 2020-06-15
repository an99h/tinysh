#include "sys.h"

void WFI_SET(void)
{
    __ASM volatile("wfi");
}

//关闭所有中断
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}

//开启所有中断
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}

//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0             //set Main Stack value
    BX r14
}

//软件复位
void SOFT_RESET_SYSTEM(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}
