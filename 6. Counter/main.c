#include "stm32f0xx.h"
#include "LCDfunctions.h"


int main(void)
{
	Init_lcd();
	/* TIM1 timer clock enable */
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	/* Counter enable */
	TIM1->CR1 |= TIM_CR1_CEN;

	/* Auto Reload Register */
	TIM1->ARR = 2000;

	/* TM1 Prescaler */
	TIM1->PSC = 65535; // 16 bit counter

	Clear_lcd();
	LCDSendaString("Counter: ");
	while(1)
	{
		LCDSetCursorLocation(0,1);
		LCDOutUDec(TIM1->CNT);
	}
}
