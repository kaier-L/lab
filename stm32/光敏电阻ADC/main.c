#include "stm32f10x.h"


void App_ADC1_Init();
int main()
{
    while(1)
    {

    }
}

void App_ADC1_Init()
{#include "stm32f10x.h"

void APP_OnBoardLED_Init();

void App_ADC1_Init();
int main()
{
	App_ADC1_Init();
	APP_OnBoardLED_Init();
	
    while(1)
    {
				ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
				
				ADC_SoftwareStartConvCmd(ADC1,ENABLE);
				
				while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
				
				uint16_t dr=ADC_GetConversionValue(ADC1);
				
				float voltage=dr*(3.3f/4095);
				
				if(voltage>1.5)
				{
				GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
				}
				else
				{
				GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
				}
				
    }
}

void App_ADC1_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef s1;
    s1.GPIO_Pin=GPIO_Pin_0;
	s1.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&s1);
	
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);


    ADC_InitTypeDef s2;
    s2.ADC_ContinuousConvMode=DISABLE;
		s2.ADC_DataAlign=ADC_DataAlign_Right;
		s2.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
		s2.ADC_Mode=ADC_Mode_Independent;
		s2.ADC_NbrOfChannel=1;
		s2.ADC_ScanConvMode=DISABLE;
		ADC_Init(ADC1,&s2);
		
		
		ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_13Cycles5);
		
		ADC_ExternalTrigConvCmd(ADC1,ENABLE);
		
		ADC_Cmd(ADC1,ENABLE);
		
		
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef s1;
    s1.GPIO_Pin=GPIO_Pin_0;
	s1.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&s1);
	
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);


    ADC_InitTypeDef s2;
    
    
		
}