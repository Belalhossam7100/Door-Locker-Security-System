/*
 * MC1.h
 *
 *  Created on: Apr 4, 2024
 *      Author: dell
 */

#ifndef MC1_H_
#define MC1_H_
#include"lcd.h"
#include"keypad.h"
#include <util/delay.h> /* For the delay functions */
#include <avr/io.h>
#include"uart_mine.h"

#define CREATED_PASS 0x10
#define RENTERED_CREATED_PASS 0x11
#define PASSWORD_CHECKING_FINISHED 0x12

#define PASSWORDS_MATCH 0x13
#define PASSWORDS_DONT_MATCH 0x14

#define OPTION_MENU 0x15

#define  OPEN_DOOR 0x16

#define USER_ENTERED_PASS 0x17
#define USER_ACCESS	0x18

#define PASSWORDS_IS_CORRECT 0x19

#define CHANGE_PASS 0x20

#define UNBLOCK_THE_SYSTEM 0x21

/*****************Functions prototypes******************************/
void Ask_user_to_enter_pass(uint8 *user_entered_password);/** for  step1 & step3 & step4**/
void create_password(uint8 *sys_password);/**step1**/
void MAIN_options(void);/**step 2**/
void Open_Door(void);/**step3**/
void change_password(void);/**step4**/
/*******************************************************************/
#endif /* MC1_H_ */
