#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "LED.h"  
#include "GLCD.h"
#include "ADC.h"
#include "KBD.h" 
 
//#include "Rock_Paper_Scissors.c"
//#include "even_odd.c"

#include "squiward.c"
#include "incognito.c" 
#include "ricardo_1.c" 
#include "ricardo_2.c" 
#include "wut.c"
#include "music_note.c"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   1										/* Uncomment to use the LCD, uses a lot of CPU power, therefore simulations, leave commented */ 

extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned char Bg_16bpp_t[];
extern unsigned char Bg_16bpp_l[];
extern unsigned char Bg_16bpp_r[];
extern unsigned char Bg_16bpp_b[];
extern unsigned char ARM_Ani_16bpp[];

uint16_t ADC_Dbg;
uint16_t kbd_actual_value = 0;  
uint32_t ad_avg = 0;
uint16_t ad_val = 0, ad_val_ = 0xFFFF;

unsigned int dir_cnt = 1; 
unsigned int menu_counter = 1;
unsigned int game_counter = 1;
unsigned int selection_enable = 0; 
unsigned int game_select = 0;
	
unsigned volatile int rst = 0; 
unsigned volatile int down_gal = 0; 
unsigned volatile int down_mp3 = 0;

void delay (unsigned long input){ //gives delay in rstement
	unsigned long int1;  
	unsigned long int2;	 
	for (int1 = input; int1 >= 1; int1 -- )
		for (int2 = 1; int2 <= input; int2 ++);  
	}	 


void menu_selection(){ 
	switch (menu_counter){  
	case 1:   
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Yellow);
		GLCD_DisplayString(4, 0, __FI, "Image Gallery");    
		GLCD_SetBackColor(White); 
		GLCD_SetTextColor(Red);
		GLCD_DisplayString(6, 0, __FI, "Music Player");    
		GLCD_DisplayString(8, 0, __FI, "Game Time");
		break;
	case 2:   
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Yellow);	
		GLCD_DisplayString(6, 0, __FI, "Music Player");  
		GLCD_SetBackColor(White);   
		GLCD_SetTextColor(Red);
		GLCD_DisplayString(8, 0, __FI, "Game Time"); 
		GLCD_DisplayString(4, 0, __FI, "Image Gallery"); 
		break;
	case 3:   
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Yellow);	
		GLCD_DisplayString(8, 0, __FI, "Game Time"); 
		GLCD_SetBackColor(White);	 
		GLCD_SetTextColor(Red);
		GLCD_DisplayString(6, 0, __FI, "Music Player");  
		GLCD_DisplayString(4, 0, __FI, "Image Gallery"); 
		break;
	}
}

/****************************************************************/
void game_selection(){ 
	switch (game_counter){  
	case 1:   
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Yellow);
		GLCD_DisplayString(4, 0, __FI, "Tic-Tac-Toe");    
		GLCD_SetBackColor(White); 
		GLCD_SetTextColor(Red);
		GLCD_DisplayString(6, 0, __FI, "Rock-Paper-Scissors");    
		GLCD_DisplayString(8, 0, __FI, "Even or Odd");
		break;
	case 2:   
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Yellow);	
		GLCD_DisplayString(6, 0, __FI, "Rock-Paper-Scissors");  
		GLCD_SetBackColor(White);   
		GLCD_SetTextColor(Red);
		GLCD_DisplayString(8, 0, __FI, "Even or Odd"); 
		GLCD_DisplayString(4, 0, __FI, "Tic-Tac-Toe"); 
		break;
	case 3:   
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Yellow);	
		GLCD_DisplayString(8, 0, __FI, "Even or Odd"); 
		GLCD_SetBackColor(White);	 
		GLCD_SetTextColor(Red);
		GLCD_DisplayString(6, 0, __FI, "Rock-Paper-Scissors");  
		GLCD_DisplayString(4, 0, __FI, "Tic-Tac-Toe"); 
		break;
	}
}

//******************************************************

void gallery (void){   
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(0, 0, __FI, "L/R Stick to shift"); 
	GLCD_DisplayString(9, 0, __FI, "Press down to exit");
	if (kbd_actual_value == 0x10)
				dir_cnt++;
	else if (kbd_actual_value == 0x40)
				dir_cnt--;
	
	if ((dir_cnt < 1)){ 
				dir_cnt = 5;
	}else if (dir_cnt > 5){ 
				dir_cnt = 1;
	}
	switch (dir_cnt){  
		case 1:  
			GLCD_Bitmap (  75, 300, 150,  150, SQUIDWARD_PIXEL_DATA);  
			break;
		case 2: 
			GLCD_Bitmap (  75, 300, 150,  150, INCOGNITO_PIXEL_DATA);  
			break; 
		case 3:       	 
			GLCD_Bitmap (  75, 300, 150,  150, RICARDO_1_PIXEL_DATA);  
			break;  
		case 4:       	 
			GLCD_Bitmap (  75, 300, 150,  150, RICARDO_2_PIXEL_DATA);  
			break;  
		case 5:  
			GLCD_Bitmap (  75, 300, 150,  150, WUT_PIXEL_DATA);  
			break;
	}  
	if (kbd_actual_value == 0x20){    
				selection_enable = 0; 
				rst = 0; 
				GLCD_Clear(White); 
				down_gal = 0;
	}
}
//******************************************************

void mp3 (void){ 
	
	int get_out;
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(0, 0, __FI, "Adj. Vol w/ Pot"); 
	GLCD_DisplayString(8, 0, __FI, "Press down to exit"); 
	
	GLCD_Bitmap (  75, 300, 150,  150,  MUSIC_NOTE_C_PIXEL_DATA);   
  get_out = audio();
	
	if (get_out == 0){  
		selection_enable = 0; 
		rst = 0; 
		GLCD_Clear(White); 
		down_mp3 = 0; 		
		}
	}
	
//******************************************************

void game(void) {
	int out = 0;
	GLCD_Clear(White);                         /* Clear graphical LCD display   */
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(Yellow);
	GLCD_DisplayString(0, 0, __FI, "   COE718 Project   ");
	GLCD_SetTextColor(White);
	GLCD_DisplayString(1, 0, __FI, "      Game Time     ");
	GLCD_DisplayString(2, 0, __FI, "   Hamzah A Qadeer  ");
	
	for (;;) {
		delay(5000);
		kbd_actual_value = get_button(); 
		if (kbd_actual_value == 0x20)
			game_counter ++;
		else if (kbd_actual_value == 0x08) 
			game_counter --;

		if (kbd_actual_value == 0x01)
			game_select = game_counter;
		
	// Ensuring a circular menu style for both menu and gallery
		if ( (game_counter < 1))
			game_counter = 3;
		else if (game_counter > 3) 
			game_counter = 1;
		
		if (game_select == 0)
			game_selection();
		else {   
			if (rst == 0)
				GLCD_Clear(White); 	  
			rst = 1;
		}
	
	if (game_select == 1 && game_counter == 1)
		ttt();  
	else if (game_select == 2 && game_counter == 2)  
		RPS();
	//else if (game_select == 3 && game_counter == 3)  
		//even_odd();
	
	if(out == 1) {
		out = 0;
		game_select = 0;
		}
	}
}

//************************************************************* 




/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {                       /* Main Program                       */
  LED_Init ();
  GLCD_Init();  
	KBD_Init();    
	ADC_Init();

	
	#ifdef __USE_LCD
		GLCD_Init();                               /* Initialize graphical LCD (if enabled */
		GLCD_Clear(White);                         /* Clear graphical LCD display   */
		GLCD_SetBackColor(Blue);
		GLCD_SetTextColor(Yellow);
		GLCD_DisplayString(0, 0, __FI, "   COE718 Project   ");
		GLCD_SetTextColor(White);
		GLCD_DisplayString(1, 0, __FI, "      Main Menu     ");
		GLCD_DisplayString(2, 0, __FI, "   Hamzah A Qadeer  ");		
	#endif

	for (;;) {
		delay(2500);
		kbd_actual_value = get_button(); 
		if (kbd_actual_value == 0x20)
			menu_counter ++;
		else if (kbd_actual_value == 0x08) 
			menu_counter --;

		if (kbd_actual_value == 0x01)
			selection_enable = menu_counter;
		
	// Ensuring a circular menu style for both menu and gallery
		if ( (menu_counter < 1))
			menu_counter = 3;
		else if (menu_counter > 3) 
			menu_counter = 1;

		if (selection_enable == 0)
			menu_selection();
	else {   
			if (rst == 0)
				GLCD_Clear(White); 	  
			rst = 1;
		}
	
	//****** If statements for choosen selection******************
	if (selection_enable == 1 && menu_counter == 1)
		gallery();  
	else if (selection_enable == 2 && menu_counter == 2)  
		mp3();
	else if  (selection_enable == 3 && menu_counter == 3)  
		game();
	}
}
