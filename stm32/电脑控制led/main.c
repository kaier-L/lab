#include "stm32f10x.h"
#include "usart.h"



int main()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
    GPIO_InitTypeDef s1={0};
    s1.GPIO_Pin=GPIO_Pin_6;
    s1.GPIO_Mode=GPIO_Mode_AF_PP;
    s1.GPIO_Speed=GPIO_Speed_10MHz;
    GPIO_Init(GPIOB,&s1);
    GPIO_InitTypeDef s2={0};
    s2.GPIO_Pin=GPIO_Pin_7;
    s2.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&s2);
    GPIO_InitTypeDef s4={0};
    s4.GPIO_Pin=GPIO_Pin_13;
    s4.GPIO_Mode=GPIO_Mode_Out_OD;
    s4.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,&s4);
    GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);

    USART_InitTypeDef s3={0};
    s3.USART_BaudRate=115200;
    s3.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;
    s3.USART_WordLength=USART_WordLength_8b;
    s3.USART_StopBits=USART_StopBits_1;
    s3.USART_Parity=USART_Parity_No;
    USART_Init(USART1,&s3);
    USART_Cmd(USART1,ENABLE);
    
    while(1)
    {
        while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
        int receive=USART_ReceiveData(USART1);
        if(receive=='0')
        {
            GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
        }
        else if(receive=='1')
        {
            GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
        }
    }
    





}