#include "sys.h"

void WFI_SET(void)
{
    __ASM volatile("wfi");
}

//�ر������ж�
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}

//���������ж�
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}

//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0             //set Main Stack value
    BX r14
}

//�����λ
void SOFT_RESET_SYSTEM(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}
