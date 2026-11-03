#include "stm32f10x.h"
void App_SystemClock_Init(void);
int main()
{
    App_SystemClock_Init();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef s1;
    s1.GPIO_Pin=GPIO_Pin_13;
    s1.GPIO_Mode=GPIO_Mode_Out_OD;
    s1.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,&s1);
    while(1)
    {
        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
        for(uint32_t i=0;i<666666;i++);
        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
        for(uint32_t i=0;i<666666;i++);
    }
}

void App_SystemClock_Init(void)
{
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_SetLatency(FLASH_Latency_2);
    // #1. 开启HSE
    RCC_HSEConfig(RCC_HSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    // #2. 配置并启动锁相环
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    // #3. 配置AHB APB1 和APB2分频器的分频系数
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);

    // #4. 切换SYSCLK的来源
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08);
}