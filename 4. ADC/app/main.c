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


#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2))
#define TEMP30_CAL_ADDR  ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define VDD_CALIB ((uint16_t) (330))
#define VDD_APPLI ((uint16_t) (300))

#define cicrle4FnC 0b11011111

int ADCTEMP(int analogValue)
{
  int temp = (((int)analogValue * VDD_APPLI / VDD_CALIB) -
               (int)*TEMP30_CAL_ADDR); 
  temp = temp * (int)(110-30);
  temp = temp / (int)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
  temp = temp + 30;
  return temp;
}

float FahtoCelsius(int analogValue)
{
  float temp = (float)(analogValue-32) / 1.8;
  return temp;
}

int main(void)
{
  int FAH_total = 0 , i = 0 , FAH = 0;
  float CEL;
  int fah_array[20];
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
  // 2. Enable internal temp sensors
  ADC->CCR |= ADC_temp_sense_enable;
  // 3. Wait till ADC is ready for conversion
  while((ADC1->ISR & ADC_ready) == 0){}
/***************************************
*         ADC Sanple Rate              *
****************************************/ 
  // 1. Set a sample rate
  ADC1->SMPR |= ADC_sample_rate;
  // 2. ADC Channel selection
  ADC1->CHSELR |= ADC_channel_16;
  // *3. voltage reference this reg is only 
  //     when using internal temp sensor
  ADC->CCR |= ADC_temp_sense_vref;
/****************************************/
  Clear_lcd();
  LCDSendaString("  Temperature");
  while(1)
  {
    // Start conversion
    ADC1->CR |= ADC_start;
    // wait till conversion is done (eoc = end of conversion)
    /////////////////////////////////////////////////////////////
    // work on this
    while((ADC1->ISR & ADC_EOC) == 0){}
    fah_array[i] = ADCTEMP(ADC1->DR);
    FAH_total += fah_array[i];
    FAH = FAH_total / i+1;
    /////////////////////////////////////////////////////////////
    CEL = FahtoCelsius(FAH);
    LCDSetCursorLocation(0,1);
    LCDOutUDec(ADCTEMP(ADC1->DR));
    LCDSetCursorLocation(4,1);
    LCDSendAnCharacter(cicrle4FnC);
    LCDSendaString("F");

    LCDSetCursorLocation(7,1);
    LCDOutFloat(CEL);
    LCDSetCursorLocation(12,1);
    LCDSendAnCharacter(cicrle4FnC);
    LCDSendaString("C");

    i = i + 1;
    if(i >= 20){i = 0;} 
    //LCDSetCursorLocation(12,1);
    //LCDOutUDec(*TEMP110_CAL_ADDR);

  }
}
