/*
 * Control_ECU.c
 *
 *  Created on: Apr 4, 2024
 *      Author: dell
 */
#include "MC2.h"

#define password_length 5
/***
 * CONTROL_ECU is responsible for all the processing and decisions in the system like created_password checking,
 * open the door and activate the system alarm
  its frequency = 8MHZ
 ***/
/************************************GLOBAL VARBS*****************************************/
uint8 counter_for_buzzer;
uint8 pass_in_EEPROM[password_length];//will be used to give the user access to use main options

/******************************Functions**************************************/
void check_created_password(uint8 *control_sys_password){/**step1**/
	uint8 reentered_created_password[password_length];
	uint8 retry_pass_control[password_length];
	uint8 counter,re_counter;
	uint8 pass_missmatch=0;

	receive_user_entered_pass(control_sys_password);

	while(UART_recieveByte() != RENTERED_CREATED_PASS){}/**first time:re-entered password**/
	for(re_counter=0;re_counter<(password_length);re_counter++)
		{
		while(UART_recieveByte() != RENTERED_CREATED_PASS){}/**first time:re-entered password**/
		reentered_created_password[re_counter] = UART_recieveByte();//Receive password
		}


	/********CHECK BOTH PASSWORDS SEND BY MC1 (MC2 RESPONSE)******************/
	for(counter=0;counter<(password_length);counter++)
	{
		if(reentered_created_password[counter]!=control_sys_password[counter])
		{
			pass_missmatch++;
			break;//no need to compare further
		}
	}
	/**************************************************************************/

	if(pass_missmatch==0)//match bet it and the saved pass in EEPROM
			{
			UART_sendByte(PASSWORD_CHECKING_FINISHED);/**second time:checking **/
			UART_sendByte(PASSWORDS_MATCH);/**second time:match **/
			/*******save password into the EEEPROM*******/
			Save_to_EEPROM(reentered_created_password);
			_delay_ms(30);//for EEPROM
			/*******************************************/
			MAIN_options_control();/**step2**/
			}
		else if(pass_missmatch!=0)//match bet it and the saved pass in EEPROM
			{
			UART_sendByte(PASSWORD_CHECKING_FINISHED);/**second time:checking**/
			UART_sendByte(PASSWORDS_DONT_MATCH);/**second time: don't match **/
			check_created_password(retry_pass_control);/**step1 again**/
			}

}
/********************************************************************************/
void MAIN_options_control(){/**step2**/
   uint8 chosen_option;//define what option is chosen on MC1
	while(UART_recieveByte() != OPTION_MENU);/**THIRD time :be ready for option menu**/
	chosen_option=UART_recieveByte();/**THIRD time**/
	if(chosen_option == OPEN_DOOR)/**THIRD time :1)open door**/
		{
		control_open_door();/**step3**/
		}
	if(chosen_option == CHANGE_PASS)/**THIRD time :2)change pass**/
		{
		control_change_password();/**step4**/
		}
}
/********************************************************************************/
void receive_user_entered_pass(uint8 *user_entered_password){/** for  step1 & step3 & step4**/
	uint8 enter_counter=0;

	while(UART_recieveByte() !=USER_ENTERED_PASS);/**FOURTH time -> be ready for the the entered pass**/
	for(enter_counter=0;enter_counter<(password_length);enter_counter++)
	{
		while(UART_recieveByte() !=USER_ENTERED_PASS);/**FOURTH time -> be ready for the the entered pass**/
		user_entered_password[enter_counter] = UART_recieveByte();//Receive the entered pass
	}
}
/********************************************************************************/
void control_open_door(){/**step3**/
	uint8 door_counter;
	uint8 door_miss_match=0;
	uint8 pass_door_control[password_length];

	receive_user_entered_pass(pass_door_control);

	Get_pass_inside_EEEPROM(pass_in_EEPROM);
	/********** MC2 Response *************/
	for(door_counter=0;door_counter<(password_length);door_counter++)
	{
		if(pass_door_control[door_counter]!=pass_in_EEPROM[door_counter])
			{
			door_miss_match++;
			break;//no need to compare further
			}
	}

	UART_sendByte(USER_ACCESS);
	if(door_miss_match==0)//PASS match bet it and the saved pass in EEPROM
	{
		UART_sendByte(PASSWORDS_IS_CORRECT);/**FIFTH time **/
		if(	locking_door==0)
		{
			DcMotor_Rotate(MOTOR_CW, 100);/**DOOR UNLOCKING FOR 15 SEC**/
		}
		if (timer1_is_init == 0)/**START timer1 for 15sec*/
		{
			Start_DcMotor_TIMER1();//timer1_is_init is now =1
		}
		/**the process of locking the door and holding it happens after 15sec
		 * when Timer1 calls DcMotor_timer_finished IN MOTOR DRIVER
		 * */
	}
	else if (door_miss_match!=0)
	{
		counter_for_buzzer++;
		UART_sendByte(PASSWORDS_DONT_MATCH);
		if(counter_for_buzzer==3)
		   {
			  	  /********ACTIVATE ALARM*********/
			  Buzzer_on();//BUZZZER ON
			  _delay_ms(30000);//keep Buzzer ON for  1 minute
			  _delay_ms(30000);
			  UART_sendByte(UNBLOCK_THE_SYSTEM);
			  counter_for_buzzer=0;//so i cna count again
			  Buzzer_off();
			  MAIN_options_control();/** back to step2**/
		   }
		control_open_door();
	}

}
/********************************************************************************/
void control_change_password(){/**step4**/
	uint8 chg_counter;
	uint8 chg_miss_match=0;
	uint8 pass_chg_control[password_length];

	uint8 new_sys_pass[password_length];

	Get_pass_inside_EEEPROM(pass_in_EEPROM);
	receive_user_entered_pass(pass_chg_control);

	/********** MC2 Response *************/
		for(chg_counter=0;chg_counter<(password_length);chg_counter++)
		{
			if(pass_chg_control[chg_counter]!=pass_in_EEPROM[chg_counter])
			{
				chg_miss_match++;
				break;//no need to compare further
			}
		}

	UART_sendByte(USER_ACCESS);
	if(chg_miss_match==0)//PASS match bet it and the saved pass in EEPROM
	{
		UART_sendByte(PASSWORDS_IS_CORRECT);/**FIFTH time **/// Update saved password in EEPROM (missing logic added);
		 check_created_password(new_sys_pass);
	}
	if (chg_miss_match!=0)
	{
		counter_for_buzzer++;
		UART_sendByte(PASSWORDS_DONT_MATCH);
		if(counter_for_buzzer==3)
		  {
			  	  /********ACTIVATE ALARM*********/
		   Buzzer_on();//BUZZZER ON
		   _delay_ms(30000);//keep Buzzer ON for  1 minute
		   _delay_ms(30000);
		   UART_sendByte(UNBLOCK_THE_SYSTEM);
		   counter_for_buzzer=0;//so i cna count again
		   Buzzer_off();
		   MAIN_options_control();/** back to step2**/
		  }
		control_change_password();
	}

}
/********************************************************************************/
void Save_to_EEPROM(uint8 *system_password){
	uint8 counter;
	//get_pass_inside_EEEPROM(saved_password); // Read the existing password from EEPROM
	/*Save_to_EEPROM(entered_password); // Update the saved password in EEPROM with the entered password
		        _delay_ms(30);
		        Get_pass_inside_EEEPROM(saved_password); // Update the saved_password array with the new password from EEPROM
		        _delay_ms(30);*/
	for(counter=0;counter<(password_length);counter++)
			{
				EEPROM_writeByte(((0x0311)+counter),system_password[counter]);
			}
	//update pass_in_EEPROM[password_length]; variable each time i save anew pass
		for(counter=0;counter<(password_length);counter++){
			pass_in_EEPROM[counter]=system_password[counter];
		}
}
/********************************************************************************/
void Get_pass_inside_EEEPROM(uint8 *saved_password){
	uint8 counter;
	for(counter=0;counter<(password_length);counter++)
				{
					EEPROM_readByte(((0x0311)+counter),&(saved_password[counter]));
				}


}
/********************************************************************************/
int main(){
		/************Initialization*******************/
		/* Initialize the EEPROM Driver */
		EEPROM_init();

		Buzzer_init();

		/*used UART frame:
	     • Date Length: 8-Bits Data
		 • Parity Type: Even Parity
		 • Stop Bits: 1-Stop Bit
		 • Baud-rate = 9600 bits/sec */
		// Setting configuration values
		 uartConfig.bit_data = eight_bit;
		 uartConfig.parity = even_parity;
		 uartConfig.stop_bit = one_Bit;
		 uartConfig.baud_rate = baud_9600;
		 // Initializing UART with the configuration
		UART_init(&uartConfig);

		DcMotor_Init();

		DcMotor_Rotate(MOTOR_OFF,0);//initial state

		uint8 entered_password[password_length];//to initialize system
		/******************************************/
  while(1)
  {

	check_created_password(entered_password);/**step1**/
  }

}
