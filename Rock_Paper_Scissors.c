#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "LED.h"  
#include "GLCD.h"
#include "ADC.h"
#include "KBD.h" 

#include "RPS.c"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   1										/* Uncomment to use the LCD, uses a lot of CPU power, therefore simulations, leave commented */ 

void delay_rps (unsigned long input){ 
	unsigned long int1;  
	unsigned long int2;	 
	for (int1 = input; int1 >= 1; int1 -- )
		for (int2 = 1; int2 <= input; int2 ++);  
	}

uint16_t kbd_value = 0;
unsigned int choice_cnt = 1; 
unsigned int user_selection = 0, comp_selection = 0;
unsigned int rest;

void choose() {
	switch (choice_cnt){  
		case 1:   
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Yellow);
			GLCD_DisplayString(7, 0, __FI, "Rock");   
			GLCD_SetBackColor(White); 
			GLCD_SetTextColor(Red);
			GLCD_DisplayString(8, 3, __FI, "Scissors"); 
			GLCD_DisplayString(7, 10, __FI, "Paper");
			break;
		case 2:   
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Yellow);	
			GLCD_DisplayString(8, 3, __FI, "Scissors");
			GLCD_SetBackColor(White);   
			GLCD_SetTextColor(Red);
			GLCD_DisplayString(7, 10, __FI, "Paper");
			GLCD_DisplayString(7, 0, __FI, "Rock"); 
			break;
		case 3:   
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Yellow);	
			GLCD_DisplayString(7, 10, __FI, "Paper");
			GLCD_SetBackColor(White);	 
			GLCD_SetTextColor(Red);
			GLCD_DisplayString(8, 3, __FI, "Scissors"); 
			GLCD_DisplayString(7, 0, __FI, "Rock"); 
			break;
	}
}

int result(int user, int comp) {
	// Win is 1
	// Lose is 2
	// Draw is 3
	int result;
	if(user == 1) {
		if (comp == 1)
			result = 3;
		else if (comp == 2)
			result = 2;
		else
			result = 1;
	}
	else if(user == 2) {
		if (comp == 1)
			result = 1;
		else if (comp == 2)
			result = 3;
		else
			result = 2;
	}
	else {
		if (comp == 1)
			result = 2;
		else if (comp == 2)
			result = 1;
		else
			result = 3;
	}
	return result;

}

void RPS() {
	// Rock is 1
	// Paper is 2
	// Scissors is 3
	LED_Init ();
 	GLCD_Init();  
	KBD_Init();    
	ADC_Init();
	
	#ifdef __USE_LCD
		GLCD_Init();                               /* Initialize graphical LCD (if enabled */
		GLCD_Clear(White);                         /* Clear graphical LCD display   */
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0, 0, __FI, "L/R Stick to shift");
		GLCD_DisplayString(10, 0, __FI, "Press down to exit");
		GLCD_Bitmap (  75, 300, 150,  150, RPS_PIXEL_DATA);
	#endif
	
	while(1) {
		delay_rps(1000);
		kbd_value = get_button();
		if (kbd_value == 0x10)
					choice_cnt++;
		else if (kbd_value == 0x40)
					choice_cnt--;
		
		if ((choice_cnt < 1))
					choice_cnt = 3;
		else if (choice_cnt > 3)
					choice_cnt = 1;
		
		if(user_selection == 0)
			choose();
		
		if(kbd_value == 0x01) {
			user_selection = choice_cnt;
			comp_selection = rand() % (3 + 1 -1) + 1;	//rand() % (max_number + 1 - minimum_number) + minimum_number
			rest = result(user_selection, comp_selection);
		}
		win_draw_lose(rest);
	}
}
