#include "stm32f10x.h"

volatile uint32_t currentTick=0;

void App_Delay(uint32_t ms);
void App_TIM3_TimeBaseInit();
void APP_OnBoardLED_Init();
int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    App_TIM3_TimeBaseInit();
    APP_OnBoardLED_Init();
    while(1)
    {
        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);

        App_Delay(50);

        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);

        App_Delay(50);
    }
}

void App_Delay(uint32_t ms)
{
    uint32_t expireTime=currentTick+ms;
    while(currentTick<expireTime);
}

void App_TIM3_TimeBaseInit()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
    TIM_TimeBaseInitStruct.TIM_Period = 999;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    TIM_Cmd(TIM3,ENABLE);

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStruct);

}

void TIM3_IRQHandler()
{
    if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET)
    {
        TIM_ClearFlag(TIM3,TIM_FLAG_Update);
        currentTick++;
    }
}

void APP_OnBoardLED_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef s1;
    s1.GPIO_Pin=GPIO_Pin_13;
    s1.GPIO_Mode=GPIO_Mode_Out_OD;
    s1.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,&s1);
    GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
    
}