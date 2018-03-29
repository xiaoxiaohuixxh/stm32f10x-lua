#include "stm32f10x.h"
void UsartDelay_ms(uint16_t time)
{    
   uint16_t i=0;
   while(time--)
   {
      i=12000; 
      while(i--);
   }
}
void InitUsartClk()
{
	
}
void InitUsartGpio(USART_TypeDef* com)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
  if (com==USART1)
  {
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
  }
  else
  {
    //RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  }

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
}
void InitUsart()
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	/* Enable USART */
  USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                  //?????? 
	
}
int usartdemo_main()
{
	InitUsartGpio(USART1);
	InitUsart();
	//while(1)
	{
		USART_SendData(USART1, (uint8_t)'t');
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
}
int usartdemo_Printf(char *str)
{
	while(*str++!='\0')
	{
		USART_SendData(USART1, (uint8_t)*str++);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
}
int usartdemo_Putchar(char str)
{
		USART_SendData(USART1, (uint8_t)str);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}