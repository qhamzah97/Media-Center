# Media-Center
Media Center created on Keil MCB 1700 microcontroller with an image gallery, media player and game center option

Image Gallery - Contains 5 images that the user can switch through
Music Player - Allows user to stream audio from a computer connected to the board, with a potentiometer being used to adjust volume
Gmae Center - Contains 3 different games (Tic-Tac-Toe, Rock-Paper-Scissors, and Even-or-Odd

The Media Center project is a simple program for the NXP 'LPC1768' 
microcontroller using Keil 'MCB1700' evaluation board and 
demonstrating interrupt functionality and display to graphic LCD.
(compliant to Cortex Microcontroller Software Interface Standard 
(CMSIS v1.0))

Example functionality:                                                   
 - Clock Settings:
   - XTAL  =  12 MHz
   - PLL0  = 400 MHz
   - CCLK  = 100 MHz
 - Displays rotating ARM 3D logo, each picture is displayed every 
   150 ms
 - 8 LEDs blink with speed of 50 ms, determined by SysTick Timer
   working in interrupt mode
 
The Blinky program is available for target:
  LPC1768 Flash: runs from Internal Flash located on chip
                 (used for production or target debugging)

Notes: - In EVAL version of uVision do not rebuild the target as 
         this example's size exceeds EVAL version limitation

Picture conversion procedure:
1. Convert picture to 16 BPP (R - 5bits, G - 6bits, B - 5bits)
2. Remove header from bmp by hex editor, remaining file size should 
   be equal to (width * height * 2) bytes
3. Convert prepared file without header to C array
