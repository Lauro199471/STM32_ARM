/*
	Pin 15 (ADC_In1) = Pot_in

*/
#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"
#include "ADC.h"

int main(void)
{
  int pot_total = 0 , i = 0 , POT = 0;
  int pot_array[20];

  // LCD
  Init_lcd();

  // ADC
  ADCInitilaze();
  ADC_ChannelSelect(1);

  Clear_lcd();
  LCDSendaString("    Pot value");

  while(1)
  {
    // Start conversion
    ADC1->CR |= ADC_start;
    // wait till conversion is done (eoc = end of conversion)
    while((ADC1->ISR & ADC_EOC) == 0){}
   
    // Smoothing
    pot_array[i] = ADC1->DR;
    pot_total = pot_total + pot_array[i];

    if(i >= 20)
    {
      i = -1;
      POT = pot_total / 21;
      LCDSetCursorLocation(6,1);
      LCDOutUDec(POT);
      pot_total = 0;
    } 
    i = i + 1;
  }
}
