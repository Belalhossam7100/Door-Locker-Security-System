/*
 * motor_mine.h
 *
 *  Created on: Feb 22, 2024
 *      Author: dell
 */

#ifndef MOTOR_MINE_H_
#define MOTOR_MINE_H_
#include "std_types.h"
#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "Timer.h"
#include <avr/io.h>
#include <util/delay.h>

#define IN2 PIN5_ID //IN PIRTB
#define IN1 PIN6_ID //IN PORTB
#define EN1 PIN3_ID //IN PORTB
//for Timer1

uint8 timer1_is_init;
uint8 locking_door;


/**motor state**/
typedef enum{
	MOTOR_OFF,
	MOTOR_CW,/*A(IN1)=0,   B(IN2)=1 */
	MOTOR_A_CW,/*A(IN1)=1,   B(IN2)=0 */
}DcMotor_State;
DcMotor_State current_state;






/**function prototypes**/
void PWM_Timer0_Start(uint8 duty_cycle);

/* Description
➢ The Function responsible for setup the direction for the two motor pins through the GPIO driver.
➢ Stop at the DC-Motor at the beginning through the GPIO driver.*/
void DcMotor_Init(void);

/*Description:
➢ The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value.
➢ Send the required duty cycle to the PWM driver based on the required speed value.*/


void DcMotor_Rotate(DcMotor_State state,uint8 speed);
void Start_DcMotor_TIMER1();
void Stop_DcMootor_TIMER1(void);

void DcMotor_timer_finished();
#endif /* MOTOR_MINE_H_ */
