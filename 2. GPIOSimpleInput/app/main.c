/***********************************************
* Description:
*		Takes a input (PC5 pin:25) and outputs to 
*   LED (PC8 pin 39)
***********************************************/
#include "stm32f0xx.h"

#define PORT_C_CLK_EN        0x00080000
#define GPIO_REG_CLEAR       0x00000000
#define GPIO_MODE_OUTPUT_C8  0x00010000
#define GPIO_MODE_INPUT_C5   0x00000C00
#define GPIO_SRC_C8          0x00000100
#define GPIO_HS_C8           0x00030000
#define GPIO_NoPUPD_C8       0x00030000
#define GPIO_NoPUPD_C5       0x00000C00
#define GPIO_C5              0x00000020
#define LED_ON               0x00000100
#define LED_OFF              0x00000100

void PortC_Init(void)
{
	// Enable the GPIO Clock for Port C using the AHB and RCC
  RCC -> AHBENR |= PORT_C_CLK_EN;

  // Set Control Registers for PC8,PC5
  // Mode Register
  GPIOC->MODER = GPIO_REG_CLEAR;
  GPIOC->MODER |= GPIO_MODE_OUTPUT_C8;
  GPIOC->MODER &= ~(GPIO_MODE_INPUT_C5);

  // Output Type Register
  GPIOC->OTYPER &= ~(GPIO_SRC_C8);

  // Output Speed Register
  GPIOC->OSPEEDR|= GPIO_HS_C8;

  // No Pull-up / Pull-down Register
  GPIOC->PUPDR &= ~(GPIO_NoPUPD_C8);
  GPIOC->PUPDR &= ~(GPIO_NoPUPD_C5);
}

int main(void)
{
	PortC_Init();
  while(1)
  {
  	if(GPIOC -> IDR & GPIO_C5)
  	{
  		//Turn ON LED
    	GPIOC->BSRR = LED_ON;
  	}
  	//Turn OFF LED
    GPIOC->BRR = LED_OFF;
  }
}
