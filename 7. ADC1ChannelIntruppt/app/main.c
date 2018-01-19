/*
	Pin 15 (ADC_In1) = Pot_in

*/
#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"
#include "ADC.h"

void ADC1_IRQHandler(void)
{
	LCDSetCursorLocation(0,1);
	LCDOutUDec(ADC1->DR);
}

int main(void)
{
  // LCD
  Init_lcd();
  Clear_lcd();
  LCDSendaString("    Pot value");

  // ADC
  ADCInitilaze();
  ADC_ChannelSelect(1);
  ADCInterruptMode();

  while(1)
  {

  }
}
