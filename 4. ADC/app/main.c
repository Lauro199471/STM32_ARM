#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"

#define ADC_CAL_bit 0x80000000
#define ADC_EN_bit  0x00000001
#define ADC_HSI14_enable  0x00000001
#define ADC_HSI14_ready   0x00000002
#define ADC_ready 0x00000001
#define ADC_interface_clk_enable  0x00000200

int main(void)
{
  PortA_Init();
  PortB_Init();
  PortC_Init();
  Init_lcd();

/***************************************
*         ADC Calibration              *
****************************************/
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
/***************************************
*         ADC Clock                    *
****************************************/
  timeDelay(1000);
  Clear_lcd();
  LCDSendaString("Start ADC CLK"); 
  // 1. Enable Clock
  RCC->APB2ENR |= ADC_interface_clk_enable;
  // 2. Enable internal RC HSI14 clock
  RCC->CR2 |= ADC_HSI14_enable; 
  // 3. Wait till HSI14RDY = 1
  while((RCC->CR2 & ADC_HSI14_ready) ==0){}

  timeDelay(1000);
  Clear_lcd();
  LCDSendaString("Success ADC CLK");   
/***************************************
*         ADC Enable                   *
****************************************/
  timeDelay(1000);
  Clear_lcd();
  LCDSendaString("Start ADC EN"); 

  // 1. ADC Enable
  ADC1->CR |= ADC_EN_bit;
  // 2. Wait till ADC is ready for a conversion
  while((ADC1->ISR & ADC_ready) == 0){}
  timeDelay(1000);
  Clear_lcd();
  LCDSendaString("ADC is ready..."); 
/****************************************/

  while(1)
  {
  }
}
