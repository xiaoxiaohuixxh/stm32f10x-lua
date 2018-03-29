#include "stm32f10x.h"
#include "leddemo.h"
#include "rtcdemo.h"
#include "usartdemo.h"
#include "pwmdemo.h"
#include "lcddemo.h"
int main()
{
	leddemo_main();
	//usartdemo_main();
	//rtcdemo_main();
	//pwmdemo_main();
	LcdDemo_main();
	while(1);
}