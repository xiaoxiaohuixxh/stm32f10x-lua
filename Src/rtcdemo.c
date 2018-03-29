#include "stm32f10x.h"
#include "usartdemo.h"
uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

void RTC_Configuration(void);
void Time_Adjust(void);
void Time_Display(uint32_t TimeVar);
int rtcdemo_main()
{
	Tmp_HH=12;
	Tmp_MM=30;
	Tmp_SS=30;
	RTC_Configuration();
	Time_Adjust();
	//while(1)
	{
		Time_Display(RTC_GetCounter());
	}
}

void Time_Adjust(void)
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter(((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS)));
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}
/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}
/**
  * @brief  Displays the current time.
  * @param  TimeVar: RTC counter value.
  * @retval None
  */
void Time_Display(uint32_t TimeVar)
{
  uint32_t THH = 0, TMM = 0, TSS = 0;
  
  /* Reset RTC Counter when Time is 23:59:59 */
  if (RTC_GetCounter() == 0x0001517F)
  {
     RTC_SetCounter(0x0);
     /* Wait until last write operation on RTC registers has finished */
     RTC_WaitForLastTask();
  }
  
  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

  usartdemo_Printf("Time:");
	usartdemo_Putchar(THH/10+0x30);
	usartdemo_Putchar(THH%10+0x30);
	usartdemo_Putchar(' ');
	usartdemo_Putchar(TMM/10+0x30);
	usartdemo_Putchar(TMM%10+0x30);
	usartdemo_Putchar(' ');
	usartdemo_Putchar(TSS/10+0x30);
	usartdemo_Putchar(TSS%10+0x30);
	usartdemo_Putchar(' ');
	usartdemo_Putchar('\r');
	usartdemo_Putchar('\n');
}