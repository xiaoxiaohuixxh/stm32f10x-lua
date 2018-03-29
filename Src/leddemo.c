#include "stm32f10x.h"
void LedDelay_ms(uint16_t time)
{    
   uint16_t i=0;
   while(time--)
   {
      i=12000; 
      while(i--);
   }
}
int leddemo_main()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//while(1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_All);
		LedDelay_ms(1000);
		//GPIO_ResetBits(GPIOA,GPIO_Pin_All);
		//LedDelay_ms(1000);
	}
}