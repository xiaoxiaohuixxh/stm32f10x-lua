#include "stm32f10x.h"

void LcdGpioInit();
int LcdInit();
void LcdWriteReg(uint16_t Addr,uint16_t Data);
uint16_t LcdReadReg(uint16_t Addr);
void LcdDataGpioSetInputMode();
void LcdDataGpioSetOutputMode();
void Delay_LCD(uint16_t n);