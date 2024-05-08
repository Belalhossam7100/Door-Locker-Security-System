/*
 * Timer.c
 *
 *  Created on: Mar 18, 2024
 *      Author: dell
 */
#include"Timer.h"
#include"Common_Macros.h"
#include"std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>


/**definition of the pointer used to call back function**/
static volatile void(*g_ptr_callBack)(void) = NULL_PTR;
uint8 timer_ticks=0;
/**NORMAL mode ISR**/
ISR(TIMER1_OVF_vect){
	if(g_ptr_callBack != NULL_PTR)
		{
		if (timer_ticks==15)
					{
			(*g_ptr_callBack)(); /* another method to call the function using pointer to function g_callBackPtr(); */

					}
		}
}

/**COMAPRE mode ISR**/
ISR(TIMER1_COMPA_vect){
	if(g_ptr_callBack != NULL_PTR)
		{
		timer_ticks++;
		if (timer_ticks==15)
			{
			(*g_ptr_callBack)(); /* another method to call the function using pointer to function g_callBackPtr(); */
			timer_ticks=0;
			}
		}
}


void Timer1_init(const Timer1_ConfigType * Config_Ptr){
	Timer1_Mode Timer_mode;
	Timer_mode = Config_Ptr->mode;
	SREG |= (1<<7);//ENABLE GLOBAL INTEREPT IN I-BIT
	SET_BIT(TCCR1A,FOC1A);//used in non_pwm mode
/**SET TIMER INTIAL VALUE**/
	TCNT1=(Config_Ptr->initial_value);
/**MODE**/
	if (Timer_mode == NORMAL){ // Normal mode
    SET_BIT(TIMSK, TOIE1); // Timer1 Overflow Interrupt is enabled.
	}
	else if (Timer_mode == CTC_TOP_OCR1A){ // Compare mode
    SET_BIT(TCCR1B, WGM12); // Use CTC mode with OCR1A as TOP
    OCR1A = (Config_Ptr->compare_value); // Set compare value
    TIMSK |= (1 << OCIE1A); // Enable Timer1 Compare A Interrupt
	}
/**PRE_SCALER**/
	TCCR1B=(TCCR1B&0xF8)|((Config_Ptr->prescaler)&0x8F);
}



void Timer1_deInit(void){
	TCCR1B=(TCCR1B|0xF8)|((0x00)&0x8F);//No clock source (Timer/Counter stopped).
	TIMSK=0; //Disable interrupt in TIME1 module
}

void Timer1_setCallBack(void(*a_ptr)(void)){
	g_ptr_callBack = a_ptr;

}

