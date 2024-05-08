/*
 * MC2.h
 *
 *  Created on: Apr 4, 2024
 *      Author: dell
 */

#ifndef MC2_H_
#define MC2_H_

#include "uart_mine.h"
#include "motor_mine.h"
#include "external_eeprom.h"
#include "Buzzer.h"
#include <util/delay.h> /* For the delay functions */
#include <avr/io.h>

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



/********************************FUNCTIONS PROTOTYPES****************************/
void check_created_password(uint8 *control_sys_password);/**step1**/
void MAIN_options_control();/**step2**/
void receive_user_entered_pass(uint8 *user_entered_password);/** for step1 & step3 & step4**/
void control_open_door();/**step3**/
void control_change_password();/**step4**/
void Save_to_EEPROM(uint8 *system_password);
void Get_pass_inside_EEEPROM(uint8 *saved_password);
/********************************************************************************/

#endif /* MC2_H_ */
