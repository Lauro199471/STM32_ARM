#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"

int main(void)
{
	PortA_Init();
  PortB_Init();
  PortC_Init();
  Init_lcd();

  
  while(1)
  {
  }
}