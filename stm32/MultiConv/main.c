#include"stm32f10x.h"
#include "usart.h"

void App_USART1_Init(void);
void App_TIM1_Init(void);
void App_ADC1_Init(void);

int main(void)
{
    App_USART1_Init();
    App_TIM1_Init();
    App_ADC1_Init();

    while(1)
    {
        while(ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC)==RESET);
        uint16_t jdr1 =ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);
        uint16_t jdr2 =ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2);

        float v1,v2;
        v1=jdr1*(3.3f/4095);
        v2=jdr2*(3.3f/4095);

        My_USART_Printf(USART1,"%.3f,%.3f\n",v1,v2);
    }
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
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

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
}

void App_TIM1_Init(void)
{
    // #1. 开启定时器1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // #2. 设置时基单元的参数
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 999;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
    TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);

    TIM_Cmd(TIM1,ENABLE);
}

void App_ADC1_Init(void)
{
        // #1. 初始化IO引脚 PA0 PA1 -> AIN
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

    ADC_InitTypeDef ADC_InitStruct = {0};
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_InjectedSequencerLengthConfig(ADC1,2);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_13Cycles5);
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_TRGO);
    ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

    ADC_Cmd(ADC1,ENABLE);

    // #3. 上电校准（不校准读数会有几十 mV 偏差）
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1) == SET);
}
