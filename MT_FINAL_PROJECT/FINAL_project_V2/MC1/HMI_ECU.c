/*
 * HMI_ECU.c
 *
 *  Created on: Apr 4, 2024
 *      Author: dell
 */
/***
 *HMI_ECU is just responsible interaction with the user
 *HMI_ECU just take inputs through keypad and display messages on the LCD.
 its frequency = 8MHZ
 ***/
/***** '='  IS used as enter button IN THE KEYPAD  **********/
#include "MC1.h"

#define password_length 5
#define delay_for_display 200
/*******************************GLOBAL VARBS**************************************/
uint8 MC2_response_on_creating_pass;
uint8 MC2_response_change_pass;
uint8 MC2_response_door;
uint8 Error_counter;
/******************************Functions**************************************/
void create_password(uint8 *sys_password){/**step1**/
	uint8 re_entered_password[password_length];//the password
	uint8 retry_pass[password_length];
	uint8 the_password_is_created;
	uint8 re_enter_counter;

	Ask_user_to_enter_pass(sys_password);

	/********re-enter create pass LCD*******/
		/* re-entering the password */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("plz re-enter ");
	LCD_moveCursor(1,0);
	LCD_displayString("same Pass:");
	/**************************************/
	/********re-enter create pass KEYPAD*******/
	for(re_enter_counter=0;re_enter_counter<(password_length);re_enter_counter++)
	{
		re_entered_password[re_enter_counter] = KEYPAD_getPressedKey();//take the password from the keypad
		LCD_displayCharacter('*');//Display * in the screen for each number
		_delay_ms(delay_for_display);//for display
	}
	while(KEYPAD_getPressedKey()!='=');//until enter key is pressed
	/****************************************/
UART_sendByte(RENTERED_CREATED_PASS);/**first time :re*entered pass**/
for(re_enter_counter=0;re_enter_counter<(password_length);re_enter_counter++)
	{
	UART_sendByte(RENTERED_CREATED_PASS);/**first time :re*entered pass**/
	UART_sendByte(re_entered_password[re_enter_counter]);//send to the Control_ECU through the UART
	}

	 /*********WAIT for MC2 Response**********/
	 /***second time:1)match  OR  2) don't match**/
	 	while(UART_recieveByte() != PASSWORD_CHECKING_FINISHED);/**second time:checking**/
	 /************Main option menu*******************/
	 	MC2_response_on_creating_pass=UART_recieveByte();//save the MC reponse

	 	if (MC2_response_on_creating_pass == PASSWORDS_MATCH)
	 	{
	 		the_password_is_created=1;
	 		MAIN_options();/**step 2**/
	 	}
	 /************Create a System Password again*******************/
	 	if (MC2_response_on_creating_pass == PASSWORDS_DONT_MATCH)
	 	{
	 		the_password_is_created=0;
	 		create_password(retry_pass);/**step1 again**/
	 	}
}
/********************************************************************************/
void MAIN_options(){/**step2**/
	/*****Main Options LCD*****/
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("+ : Open Door");
	LCD_moveCursor(1,0);
	LCD_displayString("- : Change Pass");
	/*********************************/

	UART_sendByte(OPTION_MENU);/**THIRD time :be ready for option menu**/

	if(KEYPAD_getPressedKey() == '+')//choose open door
	{
		UART_sendByte(OPEN_DOOR);/**THIRD time :1)open door**/
		Open_Door();/**step3**/
	}
	if(KEYPAD_getPressedKey() == '-')//choose Change Pass
	{
		UART_sendByte(CHANGE_PASS);/**THIRD time :2)change pass**/
		change_password();/**step4**/
	}
}
/********************************************************************************/
void Ask_user_to_enter_pass(uint8 *user_entered_password){/** for step1 & step3 & step4**/
	uint8 user_counter;
	/*****enter password LCD*****/
	LCD_sendCommand(LCD_CLEAR_COMMAND);
	LCD_displayString("plz enter pass:");
	LCD_moveCursor(1,0);
	/*********************************/
	/*****enter password KEYPAD*****/
	for(user_counter=0;user_counter<(password_length);user_counter++)
	{
		user_entered_password[user_counter] = KEYPAD_getPressedKey();//take the password from the keypad
		LCD_displayCharacter('*');//Display * in the screen for each number
		_delay_ms(delay_for_display);//for display
	}
	while(KEYPAD_getPressedKey()!='=');//until enter key is pressed
	/*********************************/

	 UART_sendByte(USER_ENTERED_PASS);/**FOURTH time -> be ready for the the entered pass**/
	 for(user_counter=0;user_counter<(password_length);user_counter++)
	 {
		UART_sendByte(USER_ENTERED_PASS);/**FOURTH time -> be ready for the the entered pass**/
		UART_sendByte(user_entered_password[user_counter]);//send to the Control_ECU through the UART
	}
}
/********************************************************************************/
void Open_Door(void){/**step3**/
	uint8 pass_open_door[password_length];
	Ask_user_to_enter_pass(pass_open_door);

	/*********WAIT for MC2 Response**********/
	/***second time:1)match  OR  2) don't match**/
	while(UART_recieveByte() != USER_ACCESS);/**second time:checking**/
	/************Main option menu*******************/
	MC2_response_door=UART_recieveByte();
	if (MC2_response_door == PASSWORDS_IS_CORRECT)//PASS match bet it and the saved pass in EEPROM
	{
	/*******************Display door state on LCD********************/
		if (timer1_is_init == 0)
		{
			Start_LCD_TIMER1();//timer1_is_init is now =1
			/**the process of locking screen and holding it happens after 15sec
			* when Timer1 calls LCD_timer_finished IN LCD DRIVER
			* */
		}
		if(lcd_closing_door==0)//Opening the door
		{
		 // Timer1 is still running, the door is opening
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_moveCursor(0,0);
			LCD_displayString("Door is Unlocking");
		}
		while(lcd_closing_door==0);
	}
	if (MC2_response_door == PASSWORDS_DONT_MATCH)
	{
		_delay_ms(1);//for synchronization
		Error_counter++;
		 if (Error_counter==3){
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_displayString("WARNING");
			while(UART_recieveByte() !=UNBLOCK_THE_SYSTEM);
			Error_counter=0;//so we can run it again
			MAIN_options();/**back to step2**/
		  }
		Open_Door();
	}


}
/********************************************************************************/
void change_password(){/**step4**/
	uint8 pass_chg_pass[password_length];
	uint8 new_pass[password_length];
	Ask_user_to_enter_pass(pass_chg_pass);
		/*********WAIT for MC2 Response**********/
		/***second time:1)match  OR  2) don't match**/
		while(UART_recieveByte() != USER_ACCESS);/**second time:checking**/
		/************Main option menu*******************/
		MC2_response_change_pass=UART_recieveByte();
		if (MC2_response_change_pass == PASSWORDS_IS_CORRECT)//PASS match bet it and the saved pass in EEPROM
		{
			create_password(new_pass);/**repeat step1**/
		}
		if (MC2_response_change_pass == PASSWORDS_DONT_MATCH){
			_delay_ms(1);//for synchronization
			Error_counter++;
			if (Error_counter==3){
				LCD_sendCommand(LCD_CLEAR_COMMAND);
				LCD_displayString("WARNING");
				while(UART_recieveByte() !=UNBLOCK_THE_SYSTEM);
				Error_counter=0;//so we can run it again
				MAIN_options();/**back to step2**/
			}
			change_password();
		}

}
/********************************************************************************/
int main(){
	/************Initialization*******************/
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

		LCD_init();

		uint8 key_pad_entered_pass[password_length];//to initialize system
	/***************************************************/
while(1)
{
	create_password(key_pad_entered_pass);/**step1**/
}

}
