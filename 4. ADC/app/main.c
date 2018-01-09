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
#define ADC_channel_16  0x00010000
#define ADC_temp_sense_enable  0x00800000
#define ADC_temp_sense_vref    0x00400000
#define bottomline    0b11000000
#define ADC_start  0x00000004
#define ADC_EOC    0x00000004


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
  // 2. Wait till ADC is ready for conversion
  while((ADC1->ISR & ADC_ready) == 0){}
  timeDelay(1000);
  Clear_lcd();
  LCDSendaString("ADC is ready...");
/***************************************
*         ADC Sanple Rate              *
****************************************/ 
  timeDelay(1000);
  Clear_lcd();
  LCDSendaString("Start ADC_SR"); 

  // 1. Set a sample rate
  ADC1->SMPR |= ADC_sample_rate;
  // 2. ADC Channel selection
  ADC1->CHSELR |= ADC_channel_16;
  // 3. Enable internal temp sensors
  ADC->CCR |= ADC_temp_sense_enable;
  // *4. voltage reference this reg is only 
  //     when using internal temp sensor
  ADC->CCR |= ADC_temp_sense_vref;

  timeDelay(1000);
  Clear_lcd();
  LCDSendaString("Temp Sensor"); 
  LCDSendAnCommand(bottomline); // go to next line
/****************************************/
  while(1)
  {
    // Start conversion
    ADC1->CR |= ADC_start;
    // wait till conversion is done (eoc = end of conversion)
    while((ADC1->ISR & ADC_EOC) == 0){}

    LCDSetCursorLocation(0,1);
    LCDOutUDec(ADC1->DR);  
  }
}
