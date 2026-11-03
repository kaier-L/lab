#include "stm32f10x.h"
#include "usart.h"

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

	GPIO_InitTypeDef s1 = {0};
	s1.GPIO_Pin   = GPIO_Pin_6;
	s1.GPIO_Mode  = GPIO_Mode_AF_PP;
	s1.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &s1);

	USART_InitTypeDef s2 = {0};
	s2.USART_BaudRate   = 115200;
	s2.USART_Mode       = USART_Mode_Tx;
	s2.USART_WordLength = USART_WordLength_8b;
	s2.USART_StopBits   = USART_StopBits_1;
	s2.USART_Parity     = USART_Parity_No;
	USART_Init(USART1, &s2);
	USART_Cmd(USART1, ENABLE);

	const uint8_t data[] = {1, 2, 3, 4, 5};
	My_USART_SendBytes(USART1, data, 5);

	while(1)
	{
	}
}