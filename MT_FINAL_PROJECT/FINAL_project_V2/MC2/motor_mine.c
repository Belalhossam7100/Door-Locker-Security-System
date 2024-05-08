/*
 * motor_mine.c
 *
 *  Created on: Feb 22, 2024
 *      Author: dell
 */
#include "motor_mine.h"
							/*****TIMER 0 PWM mode*****/
void PWM_Timer0_Start(uint8 duty_cycle){
		TCNT0 = 0; /* Set Timer Initial Value to 0*/
		/* F CPU = 1 MHZ
		 * F PWM = 500 HZ
		 * N = 8*/
		OCR0  = ((255)*(duty_cycle/(100))); /*Set Compare value*/
		DDRB  = DDRB | (1<<PB3);/*EN1  output pin for PWM*/
		/* Configure timer control register
		 * 1. Fast PWM mode FOC0=0
		 * 2. Fast PWM Mode WGM01=1 & WGM00=1
		 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
		 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
		 */
		TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}

void DcMotor_Init(void){
	/*set up pin directions*/
	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);/*EN1*/
	GPIO_setupPinDirection(PORTA_ID,PIN6_ID,PIN_OUTPUT);/*IN1*/
	GPIO_setupPinDirection(PORTA_ID,PIN5_ID,PIN_OUTPUT);/*IN2*/


	//intiial state
	timer1_is_init=0;
	locking_door=0;
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	PWM_Timer0_Start(speed);
	    if (state == MOTOR_OFF) {
	        /* Stop the motor */
	    	GPIO_writePin(PORTB_ID,PIN3_ID,LOGIC_LOW);/*EN1*/
	        GPIO_writePin(PORTA_ID, PIN6_ID, 0); /* IN1 */
	        GPIO_writePin(PORTA_ID, PIN5_ID, 0); /* IN2 */
	    } else if (state == MOTOR_CW) {
	        /* Rotate the motor clockwise */
	        GPIO_writePin(PORTA_ID, PIN6_ID, 0); /* IN1 */
	        GPIO_writePin(PORTA_ID, PIN5_ID, 1); /* IN2 */
	    } else if (state == MOTOR_A_CW) {
	        /* Rotate the motor anti-clockwise */
	        GPIO_writePin(PORTA_ID, PIN6_ID, 1); /* IN1 */
	        GPIO_writePin(PORTA_ID, PIN5_ID, 0); /* IN2 */
	    }
}



/**************************************FOR MOTOR tIMER***********************************/
void Start_DcMotor_TIMER1(){
	/************Initialize TIMER1*******************/
		timer1config.initial_value = 0;
		timer1config.compare_value =7812;
		timer1config.prescaler = PRESCALER__1024;
		timer1config.mode = CTC_TOP_OCR1A;
		Timer1_init(&timer1config);
		timer1_is_init++;
		Timer1_setCallBack(&DcMotor_timer_finished);
}



void DcMotor_timer_finished(void){
	if(locking_door==1){//DOOR IS LOCKING
		Timer1_deInit();
		DcMotor_Rotate(MOTOR_OFF,0);
	}
	else{
	DcMotor_Rotate(MOTOR_OFF,0);
	_delay_ms(3000);//hold the motor f0r 3 seconds
	DcMotor_Rotate(MOTOR_A_CW,100);//door is locked
	locking_door=1;
	}

}



