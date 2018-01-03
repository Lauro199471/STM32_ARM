/***********************************************
* Description:
*		This will make PC6 Blink an LED (PIN: 37)
***********************************************/
#include "stm32f0xx.h"

#define PORT_C_CLK_EN        0x00080000
#define GPIO_MODE_CLEAR      0x00000000
#define GPIO_MODE_OUTPUT_0_6 0x00001555
#define GPIO_OTYPE_0_6       0x0000007F
#define GPIO_OSPEEDR_0_6     0x00003FFF
#define GPIO_PUPSE_0_6       0x00003FFF

void WaitForAMoment(int time)
{
  volatile int x;
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
  GPIOC->MODER |= GPIO_MODE_OUTPUT_0_6;

  // Output Type Register
  GPIOC->OTYPER &= ~(GPIO_OTYPE_0_6);
  // Output Speed Register
  GPIOC->OSPEEDR |= GPIO_OSPEEDR_0_6;
  // Pull-up / Pull-down Register
  GPIOC->PUPDR &= ~(GPIO_PUPSE_0_6);
}

int main(void)
{
  int x = 0;
  PortC_Init();
  while(1) // Turn On and Off the LED
  {
		x = (x << 1) % 128; // set bit
		if(x == 0)
	  	x = 1;
		//Turn ON LED
    GPIOC->BSRR = x;
    // wait
    WaitForAMoment(25000);
    //Turn OFF LED
    GPIOC->BRR = x;
    //wait
    WaitForAMoment(25000);
  }
}
