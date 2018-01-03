/***************************************************************************************************************************
* - The address counter (AC) assigns addresses to both DDRAM and CGRAM.         
*                 LCD Command
*
*             Cursor Home
* - Display data RAM (DDRAM) stores the information we send to LCD in ASCII Code.
* - Using CGRAM memory the user can “build” and store their own letters.      
*  RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
*  === === === === === === === === === ===
*   0   0   0   0   0   0   0   0   1   * 
*  Returns the cursor to the home position (Address 0). Returns display to its original state if it was shifted.
*  Sets the address counter to DD RAM location 0 in the address counter. 
*
*
*               Clear Display
*  RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
*  === === === === === === === === === ===
*   0   0   0   0   0   0   0   0   0   1 
*  Clears all display and returns the cursor to the home position (Address 0). 
*
*
*             Entry Mode Set
*  RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
*  === === === === === === === === === ===
*   0   0   0   0   0   0   0   1  I/D  S 
*  Sets the effect of subsequent DD RAM read or write operations. Sets the cursor move direction and specifies or not to shift the display.
*     I/D: increment (I/D = 1) or decrement (I/D = 0) the address counter after subsequent DD RAM read or write operations. 
*       S: S = 1 the display will be shifted to the left (if I/D = 1) or right (if I/D = 0) on subsequent DD RAM write operations. 
*          This makes it looks as if the cursor stands still and the display moves when each character is written to the DD RAM. 
*          If S = 0 the display will not shift on subsequent DD RAM write operations. 
*
*             Display ON/OFF
*  RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
*  === === === === === === === === === ===
*   0   0   0   0   0   0   1   D   C   B 
*  Controls display of characters and cursor. 
*      D: The display is ON when D = 1 and OFF when D = 0. The DD RAM contents remain unchanged.
*      C: The cursor is displayed when C = 1 and is not displayed when C = 0.
*         The cursor is displayed as 5 dots in the 8th line when the 5 x 7 dot character font is 
*         selected and as 5 dots in the 11th line when the 5 x 10 dot character font is selected.
*      B: The character at the the cursor position blinks when B = 1.
*      Blinking is performed by switching between all blank dots and the display character every 409.6 ms. 
*
*
*           Cursor and Display Shift
*  RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
*  === === === === === === === === === ===
*   0   0   0   0   0   1  S/C R/L  *   * 
*  Moves the cursor and shifts the display without changing DD RAM contents 
*      S/C R/L                                          
*      === ===                                          
*       0   0   Shifts the cursor position to the left 
*               (Address Counter is decremented by 1)   
*       0   1   Shifts the cursor position to the right
*               (Address Counter is incremented by 1)  
*       1   0   Shifts the entire display to the left  
*               The cursor follows the display shift   
*       1   1   Shifts the entire display to the right 
*               The cursor follows the display shift
*
*
*       Function Set
*  RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
*  === === === === === === === === === ===
*   0   0   0   0   1   DL  N   F   *   * 
*  Sets interface data length (DL), number of display lines (N) and character font (F) 
*     DL: Sets interface data length 
*        Data sent or received in 8 bit lengths (DB7-DB0) when DL = 1
*        Data sent or received in 4 bit lengths (DB7-DB4) when DL = 0
*        When the 4 bit length is selected, data must be sent or received in pairs of 4-bits each. The most-significant 4 bits are sent or received first. 
*     N: Sets number of display lines
*     F: Sets character font
*
*    display Character  Duty                             
*  N F  lines    Font    Factor Remarks                    
*  === ======= ========= ====== =======                    
*  0 0    1    5x 7 dots  1/8    -                         
*  0 1    1    5x10 dots  1/11   -                         
*  1 *    2    5x 7 dots  1/16  Cannot display 2 lines with
*                               5x10 dot character font    
*


Set DDRAM Address

RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   0   1   A   A   A   A   A   A   A 

Sets the DD RAM address. Subsequent read or writes refer to the DD RAM. 
*/

#include "stm32f0xx.h"

#define TimeDelayBeforeEnableSet  800
#define TimeDelayBeforeDisableSet 1600
#define TimeDelayAfterDisableSet  1600

// Send D7-D0
#define CLEARLCD    0b00000001
#define CURSORHOME  0b00000010
#define ENTRYMODE   0b00000110
#define DISPLAYMODE 0b00000110
#define FUNCTIONSET 0b00111000
#define TWOLINES    0b11000000

char stringDec[5];
char res[10];
int afterpoint = 3;

void timeDelay(int wait)
{
  volatile int i = 0;
  for (i = 0; i < wait; ++i){}
}

void pinState(GPIO_TypeDef *port ,unsigned int pinNumber , uint8_t bitstate)
{
  if (bitstate)
  {
    port->BSRR|= (1 << pinNumber); // '1'
  }
  else
  {
    port->BRR|= (1 << pinNumber); // '0'
  }
}
/*******************************
*        LCD settings          *
********************************/
void LCDEnableLCD()
{
  pinState(GPIOB, 14 , 1); // Enable
}

void LCDDisableLCD()
{
  pinState(GPIOB, 14 , 0); // Enable
}

void LCDSetToWrite()
{
  pinState(GPIOB, 13 , 0); // R/W
}

void LCDSetToRead()
{
  pinState(GPIOB, 13 , 1); // R/W
}

void LCDCommandMode()
{
  pinState(GPIOB, 12 , 0); // RS
}

void LCDDataMode()
{
  pinState(GPIOB, 12 , 1); // RS
}

/*******************************
*      How to send Char        *
* - pinState                  *
*     checks bit for char and  *
*     sends the bitstate       *
* - sendByte                   *
*     does the bitChecks for   *
*     8bits                    *
********************************/

void SendByte(char x)
{
  pinState(GPIOB, 15 ,x & 0b00000001); // D0
  pinState(GPIOC, 6  ,x & 0b00000010); // D1
  pinState(GPIOC, 7  ,x & 0b00000100); // D2
  pinState(GPIOC, 8  ,x & 0b00001000); // D3
  pinState(GPIOC, 9  ,x & 0b00010000); // D4
  pinState(GPIOA, 8  ,x & 0b00100000); // D5
  pinState(GPIOA, 9  ,x & 0b01000000); // D6
  pinState(GPIOA, 10 ,x & 0b10000000); // D7
}

void LCDSendAnCharacter(char character)
{
  // (1) R/W = 0 , RS = 1 
  LCDSetToWrite();
  LCDDataMode();
  // (2) wait a min. of 40ns
  timeDelay(TimeDelayBeforeEnableSet);
  // (3) Turn ON Enable
  LCDEnableLCD();
  // (4) Output Byte
  SendByte(character);
  // (5) wait a min. of 80ns
  timeDelay(TimeDelayBeforeDisableSet);
  // (6) Tune OFF Enable
  LCDDisableLCD();
  // (7) wait a min. of 10ns
  timeDelay(TimeDelayAfterDisableSet);
  // (8)  R/W = 1
}

void LCDSendAnCommand(char CMD)
{
  // (1) R/W = 0 , RS = 0
  LCDSetToWrite();
  LCDCommandMode();
  // (2) wait a min. of 40ns
  timeDelay(TimeDelayBeforeEnableSet);
  // (3) Turn ON Enable
  LCDEnableLCD();
  // (4) Output Byte
  SendByte(CMD);
  // (5) wait a min. of 80ns
  timeDelay(TimeDelayBeforeDisableSet);
  // (6) Tune OFF Enable
  LCDDisableLCD();
  // (7) wait a min. of 10ns
  timeDelay(TimeDelayAfterDisableSet);
  // (8)  R/W = 1

}

void LCDSendaString(char* stringOut)
{
  while(*stringOut != 0) // Strings end with a NULL (Dec:0)
  {
    LCDSendAnCharacter(*stringOut++);
  }
}

/********************************
*       Range: 0 - 9999         *
*********************************/
void ConvertUDec(unsigned long n){
  if(n<10) {
    stringDec[0] = ' ';
    stringDec[1] = ' ';
    stringDec[2] = ' ';
    stringDec[3] = n%10 + '0';
  } else if(n<100) {
    stringDec[0] = ' ';
    stringDec[1] = ' ';
    stringDec[2] = (n/10)%10 + '0';
    stringDec[3] = n%10 + '0';
  } else if(n<1000) {
    stringDec[0] = ' ';
    stringDec[1] = (n/100)%10 + '0';
    stringDec[2] = (n/10)%10 + '0';
    stringDec[3] = n%10 + '0';
  }else if(n<10000) {
    stringDec[0] = n/1000 + '0';
    stringDec[1] = (n/100)%10 + '0';
    stringDec[2] = (n/10)%10 + '0';
    stringDec[3] = n%10 + '0';
  } else {
    stringDec[0] = '*';
    stringDec[1] = '*';
    stringDec[2] = '*';
    stringDec[3] = '*';
  }
  stringDec[4] = ' ';
  stringDec[5] = 0;
}
void LCDOutUDec(unsigned long n){
  ConvertUDec(n);           // convert using your function
  LCDSendaString(stringDec);// output using your function
}




/********************************
*           Float               *
*********************************/
/* Function to calculate x raised to the power y */
int power(int x, unsigned int y)
{
    if (y == 0)
        return 1;
    else if (y%2 == 0)
        return power(x, y/2)*power(x, y/2);
    else
        return x*power(x, y/2)*power(x, y/2);
}
// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}
// Converts a floating point number to string.
void LCDOutFloat(float n)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * (float)power(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
    LCDSendaString(res);
}

void Clear_lcd(void)
{
  timeDelay(200000);
  LCDSendAnCommand(CLEARLCD); 
}

void Init_lcd(void)
{
  LCDSendAnCommand(CLEARLCD); 
  LCDSendAnCommand(CURSORHOME);
  LCDSendAnCommand(FUNCTIONSET); // Two Lines 
  LCDSendAnCommand(DISPLAYMODE); // Turn on display and cursor
  LCDSendAnCommand(ENTRYMODE);   // Increment cursor to the right when writing; dont shift screen
  LCDSendaString("    Welcome");
  LCDSendAnCommand(TWOLINES); // go to next line
  LCDSendaString("  Lauro Cabral "); 
  timeDelay(800000);
}