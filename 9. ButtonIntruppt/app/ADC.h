#include "stm32f0xx.h"

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

void ADCInitilaze(void)
{
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
}


void ADC_ChannelSelect(unsigned int Channel)
{
	//  ADC Channel selection
	ADC1->CHSELR |= 1 << Channel; // ADC_inx
}

void ADCInterruptMode(void)
{
	/* Continuous conversion mode
	 *  -conversion takes place
	 *   continuously until it is
	 *   cleared
	*/
	ADC1->CFGR1 |= ADC_CFGR1_CONT;

	/* End of conversion interrupt enable
	 *  -  interrupt is generated when the EOC bit is set
	*/
	ADC1->IER |= ADC_IER_EOCIE;

	NVIC_EnableIRQ(ADC1_IRQn);
	NVIC_SetPriority(ADC1_IRQn,0);

	// ADC start conversion command
	ADC1->CR |= ADC_CR_ADSTART;
}
