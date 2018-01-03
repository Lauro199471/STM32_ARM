/***********************************************
* Description:
*		LCD project for the STM32
*   RS - PB12 (33)
*  R/W - PB13 (34)
*   E  - PB14 (35)
*		D0 - PB15 (36)
*		D1 - PC6  (37)
*		D2 - PC7  (38)
*		D3 - PC8  (39)
*		D4 - PC9  (40)
*		D5 - PA8  (41)
*		D6 - PA9  (42)
*		D7 - PA10 (43)
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
  PortA_Init();
  PortB_Init();
  PortC_Init();
  Init_lcd();
  /*while(1)
  {
    timeDelay(200000);
    LCDSendAnCommand(0b00000001); // Clear Display
    LCDSendaString("i: ");
    LCDOutFloat(i);
    LCDSendAnCommand(0b11000000); // go to next line
    LCDSendaString("j: ");
    LCDOutFloat(j);
    i = i + 0.1;
    if(i > 12)
      i = 0;
    j = j + 0.2;
    if(j > 12)
      j = 0;
  }*/
}