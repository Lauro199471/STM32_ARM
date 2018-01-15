#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"

#define PORT_C_CLK_EN        0x00080000
#define GPIO_MODE_CLEAR      0x00000000
#define GPIO_MODE_OUTPUT_6   0x00001000
#define GPIO_OTYPE_6         0x00000040
#define GPIO_OSPEEDR_6       0x00003000
#define GPIO_PUPSE_6         0x00003000

void StartHSE(void);
void RCC_CRS_IRQHandler(void);

void WaitForAMoment(int time)
{
  int x;
  for (x = 0; x < time; ++x) {
  }
}

void PortC_Init(void)
{
  // Enable the GPIO Clock for Port C using the AHB and RCC
  RCC -> AHBENR |= PORT_C_CLK_EN;

  // Set Control Registers for PC6
  // Mode Register
  GPIOC->MODER = GPIO_MODE_CLEAR;
  GPIOC->MODER |= GPIO_MODE_OUTPUT_6;

  // Output Type Register
  GPIOC->OTYPER &= ~(GPIO_OTYPE_6);
  // Output Speed Register
  GPIOC->OSPEEDR |= GPIO_OSPEEDR_6;
  // Pull-up / Pull-down Register
  GPIOC->PUPDR &= ~(GPIO_PUPSE_6);
}

/** System Clock Configuration
*/
void SystemClock_Config(void);

int main(void)
{
	int x = 64;
	SystemInit();
	PortC_Init();

	while(1) // Turn On and Off the LED
	{
		//Turn ON LED
		GPIOC->BSRR = x;
		// wait
		WaitForAMoment(16000000);
		//Turn OFF LED
		GPIOC->BRR = x;
		//wait
		WaitForAMoment(16000000);
	}

}

/**
  * Description: This function enables the interrupt on HSE ready,
  *              and start the HSE as external clock.
  */
void StartHSE(void)
{
  /* Configure NVIC for RCC */
  /* (1) Enable Interrupt on RCC */
  /* (2) Set priority for RCC */
  NVIC_EnableIRQ(RCC_CRS_IRQn);/* (1)*/
  NVIC_SetPriority(RCC_CRS_IRQn,0); /* (2) */

  /* (1) Enable interrupt on HSE ready */
  /* (2) Enable the CSS
         Enable the HSE and set HSEBYP to use the external clock
         instead of an oscillator
         Enable HSE */
  /* Note : the clock is switched to HSE in the RCC_CRS_IRQHandler ISR */
  RCC->CIR |= RCC_CIR_HSERDYIE; /* (1) */
  RCC->CR |= RCC_CR_CSSON | RCC_CR_HSEBYP | RCC_CR_HSEON;
}

/**
  * Description: This function handles RCC interrupt request
  *              and switch the system clock to HSE.
  */
void RCC_CRS_IRQHandler(void)
{
  /* (1) Check the flag HSE ready */
  /* (2) Clear the flag HSE ready */
  /* (3) Switch the system clock to HSE */
  if((RCC->CIR & RCC_CIR_HSERDYF)!=0)/* (1) */
  {
	RCC->CIR|=RCC_CIR_HSERDYC;/* (2) */
    RCC->CFGR=((RCC->CFGR&(~RCC_CFGR_SW))|RCC_CFGR_SW_0);/* (3) */
  }
  else
  {
   /* Report an error */
  }
}
