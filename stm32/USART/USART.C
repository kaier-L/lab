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