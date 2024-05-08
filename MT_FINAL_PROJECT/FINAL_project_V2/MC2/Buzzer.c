/*
 * Buzzer.c
 *
 *  Created on: Mar 19, 2024
 *      Author: dell
 */
#include"Buzzer.h"
#include "gpio.h"
#include "std_types.h"

void Buzzer_init(){
	GPIO_setupPinDirection(BUZZER_PORT,BUZZER_PIN,PIN_OUTPUT);//set up buzz pin as output pin
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);//buzzer off
}
void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_HIGH);//buzzer on
}
void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);//buzzer off
}
