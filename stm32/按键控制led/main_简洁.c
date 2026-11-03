#include "stm32f10x.h"

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef s = {0};
	s.GPIO_Pin   = GPIO_Pin_0;
	s.GPIO_Mode  = GPIO_Mode_Out_PP;
	s.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &s);

	s.GPIO_Pin  = GPIO_Pin_1;
	s.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &s);

	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		else
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	}
}