/*
*  Pin 15 (ADC_In1) = Pot_in
*  Pin 20 (ADC_In4) = Light_in
*/
#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"
#include "ADC.h"

#define GPIO_MODE_INPUT_A15 0xC0000000
int counter = 0;

void EXTI4_15_IRQHandler(void)
{
	timeDelay(5000);
	//Pending register
	// 1: selected trigger request occurred
	if(EXTI->PR & EXTI_PR_PR15)
	{
		counter = counter + 1;
		EXTI->PR |= EXTI_PR_PR15;
	}

}

int main(void)
{
  // LCD
  Init_lcd();
  Clear_lcd();
  LCDSetCursorLocation(0,0);
  LCDSendaString("Button: ");

  /* NOTE GPIO A CLK has started from LCD init*/
  // Set A15 as input
  GPIOA->MODER &= ~GPIO_MODE_INPUT_A15;
  // No Pull-up,Pull-Down
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR15;

  /*** External interrupt configuration ***/
  // Pin 15 is in EXTIC 4 which has pins Px 11-15
  SYSCFG->EXTICR[3] = SYSCFG_EXTICR2_EXTI5_PA;
  //Interrupt Mask Register (do not mask)
  EXTI->IMR |= EXTI_IMR_MR15;
  //Rising Trigger Selection Register
  EXTI->RTSR |= EXTI_RTSR_TR15;

  NVIC_EnableIRQ(EXTI4_15_IRQn);
  NVIC_SetPriority(EXTI4_15_IRQn, 0);

  while(1)
  {
	  LCDSetCursorLocation(10,0);
	  LCDOutUDec(counter);
  }
}
