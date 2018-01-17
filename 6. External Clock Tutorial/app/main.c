#include "stm32f0xx.h"

#define PORT_C_CLK_EN        0x00080000
#define GPIO_MODE_CLEAR      0x00000000
#define GPIO_MODE_OUTPUT_6   0x00001000
#define GPIO_OTYPE_6         0x00000040
#define GPIO_OSPEEDR_6       0x00003000
#define GPIO_PUPSE_6         0x00003000



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


int main(void)
{
	int x = 64;
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

