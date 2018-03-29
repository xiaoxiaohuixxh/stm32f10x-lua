#include "stm32f10x.h"
#include "lcd.h"
int LcdDemo_main()
{
	LcdGpioInit();
	LcdInit();
}