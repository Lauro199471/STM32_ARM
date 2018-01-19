/*
*  Pin 15 (ADC_In1) = Pot_in
*  Pin 20 (ADC_In4) = Light_in
*/
#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"
#include "ADC.h"

volatile  int channel = 0;
volatile  int channel_1_Data = 0;
volatile  int channel_4_Data = 0;

void ADC1_IRQHandler(void)
{
	if(channel == 2) channel = 0;
	if(channel == 0) channel_1_Data = ADC1->DR;
	if(channel == 1) channel_4_Data = ADC1->DR;
	channel++;
}

int main(void)
{
  // LCD
  Init_lcd();
  Clear_lcd();
  LCDSetCursorLocation(0,0);
  LCDSendaString("POT: ");

  LCDSetCursorLocation(0,1);
  LCDSendaString("LIGHT: ");
  // ADC
  ADCInitilaze();
  ADC_ChannelSelect(1);
  ADC_ChannelSelect(4);
  ADCInterruptMode();
  Clear_lcd();
  while(1)
  {
	  LCDSetCursorLocation(0,0);
	  LCDOutUDec(channel_1_Data);

	  LCDSetCursorLocation(0,1);
	  LCDOutUDec(channel_4_Data);
  }
}
