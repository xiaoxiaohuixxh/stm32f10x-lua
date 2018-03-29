#include "stm32f10x.h"
int pwmdemo_main()
{
		//config rcc
		/* TIM1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE|
													 RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);
		//init gpio
		GPIO_InitTypeDef GPIO_InitStructure;
		/* GPIOE Configuration: Channel 1/1N, 2/2N, 3/3N and 4 as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* TIM1 Full remapping pins */
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);//复用，功能选择
		//init timer
		uint16_t TimerPeriod = 0;
		TimerPeriod = (SystemCoreClock / 17570 ) - 1;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		/* Time Base configuration */
		TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period = TimerPeriod;
		TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
		TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
		
		//set time pulse
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		uint16_t Channel1Pulse = 0;
		/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
		Channel1Pulse = (uint16_t) (((uint32_t) 60 * (TimerPeriod - 1)) / 100);

		/* Channel 1, 2,3 and 4 Configuration in PWM mode */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择定时器模式
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//选择输出比较状态
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//选择互补输出比较状态
		TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;//反转时机,设置带装入的捕获比较器的脉冲值
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//设置输出极性
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//设置互补输出极性
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//选择空闲状态下的非工作状态
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;//选择互补空闲状态下非工作状态

		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
		/* TIM1 counter enable */
		TIM_Cmd(TIM1, ENABLE);

		/* TIM1 Main Output Enable */
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
		//while(1);
}