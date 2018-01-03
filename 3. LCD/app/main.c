/***********************************************
* Description:
*	LCD project for the STM32
*  RS - PB12 (33)
*  R/W - PB13 (34)
*  E  - PB14 (35)
*  D0 - PB15 (36)
*  D1 - PC6  (37)
*  D2 - PC7  (38)
*  D3 - PC8  (39)
*  D4 - PC9  (40)
*  D5 - PA8  (41)
*  D6 - PA9  (42)
*  D7 - PA10 (43)
*
***********************************************/
#include "stm32f0xx.h"
#include "port_initialize.h"
#include "LCDfunctions.h"

/*******************************
* 	        main               *
********************************/
int main(void)
{
  int y = 0, x = 0;
  PortA_Init();
  PortB_Init();
  PortC_Init();
  Init_lcd();

  for (y = 0; y <= 1; y=y+1)
  {
    for (x = 0; x <= 15; x=x+1)
    {
    	LCDSetCursorLocation(x,y);
    	LCDSendAnCharacter("x");
    	timeDelay(500000);
    	LCDSetCursorLocation(x,y);
    	LCDSendAnCharacter(" ");
	}
  }
}
