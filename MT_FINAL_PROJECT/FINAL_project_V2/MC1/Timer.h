/*
 * Timer.h
 *
 *  Created on: Mar 18, 2024
 *      Author: dell
 */

#ifndef TIMER_H_
#define TIMER_H_
#include"std_types.h"


						/**CONFIGUARTION**/
typedef enum{
NO_CLOCK_SOURCE=0,PRESCALER_1=1,PRESCALER__8=2,PRESCALER__64=3,PRESCALER__256=4,PRESCALER__1024=5
}Timer1_Prescaler;

typedef enum{//Supports normal and compare mode
NORMAL=0,
CTC_TOP_OCR1A=4,
}Timer1_Mode;

typedef struct {
uint16 initial_value;
uint16 compare_value; // it will be used in compare mode only.
Timer1_Prescaler prescaler;
Timer1_Mode mode;
} Timer1_ConfigType;

Timer1_ConfigType timer1config;



/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/* Description
 Function to initialize the Timer driver
 Inputs: pointer to the configuration structure with type Timer1_ConfigType.
 Return: None
*/
void Timer1_init(const Timer1_ConfigType * Config_Ptr);



/*
 * Description
 Function to disable the Timer1.
 Inputs: None
 Return: None
*/
void Timer1_deInit(void);




/*
 * Description
 Function to set the Call Back function address.
 Inputs: pointer to Call Back function.
 Return: None
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
