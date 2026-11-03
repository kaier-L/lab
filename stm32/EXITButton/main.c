#include "stm32f10x.h"


void APP_OnBoardLED_Init();
void App_Button_Init();


int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    APP_OnBoardLED_Init();
    App_Button_Init();
    while(1)
    {

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

void App_Button_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);


    // #3. 初始化EXTI的线
    EXTI_InitTypeDef EXTI_InitStruct;

    EXTI_InitStruct.EXTI_Line = EXTI_Line5;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    EXTI_InitStruct.EXTI_Line = EXTI_Line6;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);


    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}



void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
    {
        EXTI_ClearFlag(EXTI_Line5);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // 亮灯
    }

    if(EXTI_GetFlagStatus(EXTI_Line6) == SET)
    {
        EXTI_ClearFlag(EXTI_Line6);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); // 灭灯
    }

}

