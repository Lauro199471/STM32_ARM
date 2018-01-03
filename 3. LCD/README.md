            Clear Display
RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0

=== === === === === === === === === ===
0     0     0     0   0   0   0   0   0   1 

Clears all display and returns the cursor to the home position (Address 0).

Details
Writes space code (20h) into all DD RAM addresses. Sets address counter to DD RAM location 0. Returns display to its original state if it was shifted. In other words, the display disappears and the cursor or blink goes to the left edge of the display (the first line if 2 lines are displayed). Sets entry mode I/D to 1 (Increment Mode). Entry mode shift (S) bit remains unchanged.

Execution Time = 82µs-1.64ms / 120µs-4.9ms
--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------
            Cursor Home
RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   0   0   0   0   0   0   0   1   * 

Returns the cursor to the home position (Address 0). Returns display to its original state if it was shifted.

Details
Sets the address counter to DD RAM location 0 in the address counter. Returns display to its original state if it was shifted. DD RAM contents remain unchanged. The cursor or blink goes to the left edge of the display (the first line if 2 lines are displayed).

Execution Time = 40µs-1.6ms / 120µs-4.8ms
--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------
            Entry Mode Set
RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   0   0   0   0   0   0   1  I/D  S 

Sets the effect of subsequent DD RAM read or write operations. Sets the cursor move direction and specifies or not to shift the display. These operations are performed during data read and write.

Details
Specifies whether to increment (I/D = 1) or decrement (I/D = 0) the address counter after subsequent DD RAM read or write operations.

If S = 1 the display will be shifted to the left (if I/D = 1) or right (if I/D = 0) on subsequent DD RAM write operations. This makes it looks as if the cursor stands still and the display moves when each character is written to the DD RAM. if S = 0 the display will not shift on subsequent DD RAM write operations.

Execution Time = 40µs / 120µs
--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------
            Display ON/OFF
RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   0   0   0   0   0   1   D   C   B 

Controls display of characters and cursor.

Details
D: The display is ON when D = 1 and OFF when D = 0. The DD RAM contents remain unchanged.

C: The cursor is displayed when C = 1 and is not displayed when C = 0.
The cursor is displayed as 5 dots in the 8th line when the 5 x 7 dot character font is selected and as 5 dots in the 11th line when the 5 x 10 dot character font is selected.

B: The character at the the cursor position blinks when B = 1.
Blinking is performed by switching between all blank dots and the display character every 409.6 ms.

Execution Time = 40µs / 120µs
--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------
            Cursor and Display Shift
RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   0   0   0   0   1  S/C R/L  *   * 

Moves the cursor and shifts the display without changing DD RAM contents

Details
Shifts cursor position or display to the right or left without writing or reading display data. This function is used to correct or search for the display. In a 2-line display, the cursor moves to the 2nd line when it passes the 40th digit of the 1st line. Notice that the 1st and 2nd line displays will shift at the same time. When the displayed data is shifted repeatedly each line only moves horizontally. The 2nd line of the display does not shift into the 1st line position.

S/C R/L                                          
=== ===                                          
 0   0   Shifts the cursor position to the left 
         (Address Counter is decremented by 1)   
 0   1   Shifts the cursor position to the right
         (Address Counter is incremented by 1)  
 1   0   Shifts the entire display to the left  
         The cursor follows the display shift   
 1   1   Shifts the entire display to the right 
         The cursor follows the display shift   

Execution Time = 40µs / 120µs
--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------
            Function Set
RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   0   0   0   1   DL  N   F   *   * 

Sets interface data length (DL), number of display lines (N) and character font (F)

Details
This command should be issued only after automatic power-on initialization has occurred, or as part of the module initialization sequence.
DL: Sets interface data length

    Data sent or received in 8 bit lengths (DB7-DB0) when DL = 1

    Data sent or received in 4 bit lengths (DB7-DB4) when DL = 0

    When the 4 bit length is selected, data must be sent or received in pairs of 4-bits each. The most-significant 4 bits are sent or received first. 

N: Sets number of display lines

F: Sets character font

    display Character  Duty                             
N F  lines    Font    Factor Remarks                    
=== ======= ========= ====== =======                    
0 0    1    5x 7 dots  1/8    -                         
0 1    1    5x10 dots  1/11   -                         
1 *    2    5x 7 dots  1/16  Cannot display 2 lines with
                             5x10 dot character font    

Note that a 1 line x 16 character display is treated as a 2 line x 8 character display. The first `line' is the left eight character positions on the display and the second `line' is the right eight character positions. This also means that the 16 characters on the display do not occupy 16 sequential DD RAM locations.
Execution Time = 40µs / 120µs

Set CG RAM Address

RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   0   0   1   A   A   A   A   A   A 

Sets the CG RAM address. Subsequent read or write operations refer to the CG RAM.

Details
Sets the specified value (AAAAAA) into the address counter. Subsequent read or write operations transfer data from, or to, the character generator RAM.

Execution Time = 40µs / 120µs

Set DDRAM Address

RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   0   1   A   A   A   A   A   A   A 

Sets the DD RAM address. Subsequent read or writes refer to the DD RAM.

Details
Sets the specified value (AAAAAAA) into the address counter. Subsequent read or write operations transfer data from, or to, the display RAM. Note: Adjacent display RAM locations do not necessarily refer to adjacent display positions.

Execution Time = 40µs / 120µs

Read busy flag and address counter

RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 0   1   BF  A   A   A   A   A   A   A 

Reads the state of the busy flag (BF) and the contents of the address counter.

Details
Reads the busy flag (BF) that indicates the state of the LCD module. BF = 1 indicates that the module is busy processing the previous command. BF = 0 indicates that the module is ready to perform another command.

The value of the address counter is also returned. The same address counter is used for both CG and DD RAM transfers.

This command can be issued at any time. It is the only command which the LCD module will accept while a previous command is still being processed.
Execution Time = 1µs

Write data to CG or DD RAM

RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 1   0   D   D   D   D   D   D   D   D 

Writes data into DD RAM or CG RAM

Details
Writes a byte (DDDDDDDD) to the CG or the DD RAM. The destination (CG RAM or DD RAM) is determined by the most recent `Set RAM Address' command. The location to which the byte will be written is the current value of the address counter. After the byte is written the address counter is automatically incremented or decremented according to the entry mode. The entry mode also determines whether or not the display will shift.

Execution Time = 40µs / 120µs

Read data from CG or DD RAM

RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
=== === === === === === === === === ===
 1   1   D   D   D   D   D   D   D   D 

Reads data from DD RAM or CG RAM.

Details
Reads a byte (DDDDDDDD) from the CG or DD RAM. The source (CG RAM or DD RAM) is determined by the most recent `Set RAM Address' command. The location from which the byte will be read is the current value of the address counter. After the byte is reda the address counter is automatically incremented or decremented according to the entry mode.

Execution Time = 40µs / 120µs 
