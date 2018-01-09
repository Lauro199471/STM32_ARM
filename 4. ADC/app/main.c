#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"

#define ADC_CAL_bit 0x80000000
#define ADC_EN_bit  0x00000001

int main(void)
{
  PortA_Init();
  PortB_Init();
  PortC_Init();
  Init_lcd();

  Clear_lcd();
  LCDSendaString("Start ADC CAL"); 

  // 1. Disable ADC Enable
  ADC1->CR &= ~ADC_EN_bit;
  // 2. Enable ADC Calibrate
  ADC1->CR |= ADC_CAL_bit;
  // 3. Wait till ADCAL = 0
  while((ADC1->CR & ADC_CAL_bit) !=0){}
    
  timeDelay(1000);
  Clear_lcd();
  LCDSendaString("Success ADC CAL"); 

  while(1)
  {
  }
}
