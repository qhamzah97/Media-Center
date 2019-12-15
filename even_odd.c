#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "LED.h"  
#include "GLCD.h"
#include "ADC.h"
#include "KBD.h" 


#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   1										/* Uncomment to use the LCD, uses a lot of CPU power, therefore simulations, leave commented */ 

void delay_eo (unsigned long input){ 
	unsigned long int1;  
	unsigned long int2;	 
	for (int1 = input; int1 >= 1; int1 -- )
		for (int2 = 1; int2 <= input; int2 ++);  
	}

uint16_t kbd_value_eo = 0;
unsigned int choice_cnt_eo = 1; 
unsigned int user_selection_eo = 0, comp_choice = 0;;
unsigned int rest_eo;

void choose_eo() {
	switch (choice_cnt_eo){  
		case 1:   
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Yellow);
			GLCD_DisplayString(7, 0, __FI, "Even");   
			GLCD_SetBackColor(White); 
			GLCD_SetTextColor(Red);
			GLCD_DisplayString(7, 7, __FI, "Odd");
			break;
		case 2:   
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Yellow);	
			GLCD_DisplayString(7, 7, __FI, "Odd");
			GLCD_SetBackColor(White);   
			GLCD_SetTextColor(Red);
			GLCD_DisplayString(7, 0, __FI, "Even"); 
			break;
	}
}

int result_eo(int user, int comp) {
	int result, even_or_odd;
	even_or_odd = comp % 2;
	if(user == 1) {
		if (even_or_odd == 1)
			result = 2;
		else if (even_or_odd == 0)
			result = 1;
	}
	else if(user == 2) {
		if (even_or_odd == 1)
			result = 1;
		else if (even_or_odd == 0)
			result = 2;
	}
	return result;
}

void even_odd() {
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
	#endif

	
	while(1) {
		delay_eo(1000);
		kbd_value_eo = get_button();
		if (kbd_value_eo == 0x10)
					choice_cnt_eo++;
		else if (kbd_value_eo == 0x40)
					choice_cnt_eo--;
		
		if ((choice_cnt_eo < 1))
					choice_cnt_eo = 2;
		else if (choice_cnt_eo > 2)
					choice_cnt_eo = 1;

		if(user_selection_eo == 0)
			choose_eo();
		
		if(kbd_value_eo == 0x01) {
			user_selection_eo = choice_cnt_eo;
			comp_choice = rand();
			rest_eo = result_eo(user_selection_eo, comp_choice);
		}

		win_draw_lose(rest_eo);
	}
}
