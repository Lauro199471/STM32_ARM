/*
*  Pin 15 (ADC_In1) = Pot_in
*  Pin 20 (ADC_In4) = Light_in
*/
#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"
#include "ADC.h"

volatile  int channel_1_Data = 0;

void ADC1_IRQHandler(void)
{
	channel_1_Data = ADC1->DR;
}

int main(void)
{
  // LCD
  Init_lcd();
  Clear_lcd();
  LCDSetCursorLocation(0,0);
  LCDSendaString("POT: ");
  // ADC
  ADCInitilaze();
  ADC_ChannelSelect(1);
  ADCInterruptMode();
  Clear_lcd();
  while(1)
  {
	  LCDSetCursorLocation(0,0);
	  LCDOutUDec(channel_1_Data);
  }
}
