/*
	Pin 15 (ADC_In1) = Pot_in

*/
#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"

#define ADC_CAL_bit 0x80000000
#define ADC_EN_bit  0x00000001
#define ADC_HSI14_enable  0x00000001
#define ADC_HSI14_ready   0x00000002
#define ADC_ready 0x00000001
#define ADC_interface_clk_enable  0x00000200
#define ADC_sample_rate 0x00000007
#define ADC_channel_1  0x00000002
#define ADC_temp_sense_enable  0x00800000
#define ADC_temp_sense_vref    0x00400000
#define bottomline    0b11000000
#define ADC_start  0x00000004
#define ADC_EOC    0x00000004


int main(void)
{
  int pot_total = 0 , i = 0 , POT = 0;
  int pot_array[20];
  PortA_Init();
  PortB_Init();
  PortC_Init();
  Init_lcd();

/***************************************
*         ADC Calibration              *
****************************************/
  // 1. Disable ADC Enable
  ADC1->CR &= ~ADC_EN_bit;
  // 2. Enable ADC Calibrate
  ADC1->CR |= ADC_CAL_bit;
  // 3. Wait till ADCAL = 0
  while((ADC1->CR & ADC_CAL_bit) !=0){}
/***************************************
*         ADC Clock                    *
****************************************/
  // 1. Enable Clock
  RCC->APB2ENR |= ADC_interface_clk_enable;
  // 2. Enable internal RC HSI14 clock
  RCC->CR2 |= ADC_HSI14_enable; 
  // 3. Wait till HSI14RDY = 1
  while((RCC->CR2 & ADC_HSI14_ready) ==0){}  
/***************************************
*         ADC Enable                   *
****************************************/
  // 1. ADC Enable
  ADC1->CR |= ADC_EN_bit;
  // 2. Wait till ADC is ready for conversion
  while((ADC1->ISR & ADC_ready) == 0){}
/***************************************
*         ADC Sanple Rate              *
****************************************/ 
  // 1. Set a sample rate
  ADC1->SMPR |= ADC_sample_rate;
  // 2. ADC Channel selection
  ADC1->CHSELR |= ADC_channel_1; // ADC_in1
/****************************************/
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
