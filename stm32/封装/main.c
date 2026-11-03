#include "stm32f10x.h"
#include "usart.h"
void My_USART1_Init();
int main()
{
    My_USART1_Init();
    // My_USART_SendByte(USART1,0X3F);
    // uint8_t bytearray[]={1,2,3,4,5};
    // My_USART_SendBytes(USART1,bytearray,5);
    // My_USART_SendChar(USART1,'A');
    // My_USART_SendString(USART1,"HELLO WORLD \r\n");
    // const char *strname="tom";
    // My_USART_Printf(USART1,"Hi,%s!nice to meet you!\r\n",strname);
    // GPIO_InitTypeDef s1;
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    // s1.GPIO_Pin=GPIO_Pin_13;
    // s1.GPIO_Mode=GPIO_Mode_Out_OD;
    // s1.GPIO_Speed=GPIO_Speed_2MHz;
    // GPIO_Init(GPIOC,&s1);
    while(1)
    {
        // char c=My_USART_ReceiveByte(USART1);
        // if(c=='0')
        // {
        //     GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);

        // }
        // else if(c=='1')
        // {
        //     GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
        // }
        char buffer[100];
        if(My_USART_ReceiveLine(USART1,buffer,100,LINE_SEPERATOR_CRLF,-1)==0)
        {
            My_USART_SendString(USART1,buffer);
        }

    }
}

void My_USART1_Init()
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
     USART_InitTypeDef s3={0};
    s3.USART_BaudRate=115200;
    s3.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;
    s3.USART_WordLength=USART_WordLength_8b;
    s3.USART_StopBits=USART_StopBits_1;
    s3.USART_Parity=USART_Parity_No;
    USART_Init(USART1,&s3);
    USART_Cmd(USART1,ENABLE);
}