#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
uint32_t blink=1000;
void APP_OnBoardLED_Init();
void App_USART1_Init(void);
int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    APP_OnBoardLED_Init();
    App_USART1_Init();

    My_USART_SendString(USART1,"HELLO");
    while(1)
    {
        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
        Delay(blink);
        GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
        Delay(blink);
    }
}


void USART1_IRQHandler()
{
    if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
    {
    uint8_t byte=USART_ReceiveData(USART1);

    if(byte=='0')
    {
        blink=1000;
    }
    else if(byte=='1')
    {
        blink=200;
    }
    else if(byte=='2')
    {
        blink=50;
    }
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
}

void App_USART1_Init(void)
{
    // #1. 初始化IO引脚

    GPIO_InitTypeDef GPIO_InitStruct;

    // PA9 AF_PP
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PA10 IPU
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 初始化USART1
    USART_InitTypeDef USART_InitStruct;

    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;

    USART_Init(USART1, &USART_InitStruct);

    // 闭合总开关
    USART_Cmd(USART1, ENABLE);

    //配置中断
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

    //配置NVIC
    NVIC_InitTypeDef s4;
    s4.NVIC_IRQChannel=USART1_IRQn;
    s4.NVIC_IRQChannelPreemptionPriority=0;
    s4.NVIC_IRQChannelSubPriority=0;
    s4.NVIC_IRQChannelCmd=ENABLE;

    NVIC_Init(&s4);


}