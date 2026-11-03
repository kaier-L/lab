#include"stm32f10x.h"
#include"usart.h"
#include"button.h"

void button_clicked_cb(uint8_t clicks);
void button_long_pressed_cb(uint8_t ticks);
Button_TypeDef button1;
uint8_t cnt=0;

int main()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef s1 = {0};
	s1.GPIO_Pin   = GPIO_Pin_9;
	s1.GPIO_Mode  = GPIO_Mode_AF_PP;
	s1.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &s1);

    GPIO_InitTypeDef s2 = {0};
	s2.GPIO_Pin   = GPIO_Pin_10;
	s2.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &s2);

	USART_InitTypeDef s3 = {0};
	s3.USART_BaudRate   = 115200;
	s3.USART_Mode       = USART_Mode_Tx;
	s3.USART_WordLength = USART_WordLength_8b;
	s3.USART_StopBits   = USART_StopBits_1;
	s3.USART_Parity     = USART_Parity_No;
	USART_Init(USART1, &s3);
	USART_Cmd(USART1, ENABLE);

   

    Button_InitTypeDef Button_InitStruct;
    Button_InitStruct.GPIOx = GPIOA;
    Button_InitStruct.GPIO_Pin = GPIO_Pin_0;
    Button_InitStruct.ClickInterval = 0;
    Button_InitStruct.LongPressTime = 0;
    Button_InitStruct.LongPressTickInterval = 0;
    Button_InitStruct.button_clicked_cb =button_clicked_cb;
    Button_InitStruct.button_pressed_cb = 0;
    Button_InitStruct.button_released_cb = 0;
    Button_InitStruct.button_long_pressed_cb = button_long_pressed_cb;
    My_Button_Init(&button1,&Button_InitStruct);


    while(1)
    {
        My_Button_Proc(&button1);
    }
}


void button_clicked_cb(uint8_t clicks)
{
    if(clicks==1)
    {
        cnt++;
        My_USART_Printf(USART1,"%d",cnt);
    }
    else if(clicks==2)
    {
        cnt=0;
        My_USART_Printf(USART1,"%d",cnt);
    }
}

void button_long_pressed_cb(uint8_t ticks)
{
    cnt++;
    My_USART_Printf(USART1,"%d",cnt);
}