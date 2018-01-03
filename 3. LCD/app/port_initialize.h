#include "stm32f0xx.h"

#define PORT_C_CLK_EN        0x00080000
#define PORT_B_CLK_EN        0x00040000
#define PORT_A_CLK_EN        0x00020000

// Port C
#define GPIO_MODE_CLEAR_C6_C9   0x000FF000
#define GPIO_MODE_OUTPUT_C6_C9  0x00055000

#define GPIO_TYPE_CLEAR_C6_C9  0x000003C0
#define GPIO_TYPE_SRC_C6_C9    0x00000000

#define GPIO_OSPEEDR_CLEAR_C6_C9  0x000FF000
#define GPIO_OSPEEDR_HIGH_C6_C9   0x000FF000

#define GPIOC_CLEAR_PUPDR     0x000FF000
#define GPIO_NoPUPD_C6_C9    0x00000000

// Port B
#define GPIO_MODE_CLEAR_B12_B15  0xFF000000
#define GPIO_MODE_OUTPUT_B12_B15 0x55000000

#define GPIO_TYPE_CLEAR_B12_B15  0xF0000000
#define GPIO_TYPE_SRC_B12_B15    0x00000000

#define GPIO_OSPEEDR_CLEAR_B12_B15  0xFF000000
#define GPIO_OSPEEDR_HIGH_B12_B15   0xFF000000

#define GPIOB_CLEAR_PUPDR     0xFF000000
#define GPIO_NoPUPD_B12_B15  0x00000000

// Port A
#define GPIO_MODE_CLEAR_A8_A10  0x003F0000
#define GPIO_MODE_OUTPUT_A8_A10 0x00150000

#define GPIO_TYPE_CLEAR_A8_A10  0x00000700
#define GPIO_TYPE_SRC_A8_A10    0x00000000

#define GPIO_OSPEEDR_CLEAR_A8_A10  0x003F0000
#define GPIO_OSPEEDR_HIGH_A8_A10   0x003F0000

#define GPIOA_CLEAR_PUPDR   0x003F0000
#define GPIO_NoPUPD_A8_A10  0x00000000

/*******************************
*       Port Initilazing       *
********************************/
void PortC_Init(void)
{
  // Enable the GPIO Clock for Port C using the AHB and RCC
  RCC -> AHBENR |= PORT_C_CLK_EN;

  // Set Control Registers for PC6-9
  // Mode Register
  GPIOC->MODER &= ~(GPIO_MODE_CLEAR_C6_C9);
  GPIOC->MODER |= GPIO_MODE_OUTPUT_C6_C9;

  // Output Type Register
  GPIOC->OTYPER &= ~(GPIO_TYPE_CLEAR_C6_C9);
  GPIOC->OTYPER |= GPIO_TYPE_SRC_C6_C9;

  // Output Speed Register
  GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_CLEAR_C6_C9);
  GPIOC->OSPEEDR |= GPIO_OSPEEDR_HIGH_C6_C9;

  // No Pull-up / Pull-down Register
  GPIOC->PUPDR &= ~(GPIOC_CLEAR_PUPDR);
  GPIOC->PUPDR |= GPIO_NoPUPD_C6_C9;
}

void PortB_Init(void)
{
  // Enable the GPIO Clock for Port B using the AHB and RCC
  RCC -> AHBENR |= PORT_B_CLK_EN;

  // Set Control Registers for PC8,PC5
  // Mode Register
  GPIOB->MODER &= ~(GPIO_MODE_CLEAR_B12_B15);
  GPIOB->MODER |= GPIO_MODE_OUTPUT_B12_B15;

  // Output Type Register
  GPIOB->OTYPER &= ~(GPIO_TYPE_CLEAR_B12_B15);
  GPIOB->OTYPER |= GPIO_TYPE_SRC_B12_B15;

  // Output Speed Register
  GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_CLEAR_B12_B15);
  GPIOB->OSPEEDR |= GPIO_OSPEEDR_HIGH_B12_B15;

  // No Pull-up / Pull-down Register
  GPIOB->PUPDR &= ~(GPIOB_CLEAR_PUPDR);
  GPIOB->PUPDR |= GPIO_NoPUPD_B12_B15;
}

void PortA_Init(void)
{
  // Enable the GPIO Clock for Port A using the AHB and RCC
  RCC -> AHBENR |= PORT_A_CLK_EN;

  // Set Control Registers for PA8-10
  // Mode Register
  GPIOA->MODER &= ~(GPIO_MODE_CLEAR_A8_A10);
  GPIOA->MODER |= GPIO_MODE_OUTPUT_A8_A10;

  // Output Type Register
  GPIOA->OTYPER &= ~(GPIO_TYPE_CLEAR_A8_A10);
  GPIOA->OTYPER |= GPIO_TYPE_SRC_A8_A10;

  // Output Speed Register
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_CLEAR_A8_A10);
  GPIOA->OSPEEDR |= GPIO_OSPEEDR_HIGH_A8_A10;

  // No Pull-up / Pull-down Register
  GPIOA->PUPDR &= ~(GPIOA_CLEAR_PUPDR);
  GPIOA->PUPDR |= GPIO_NoPUPD_A8_A10;
}