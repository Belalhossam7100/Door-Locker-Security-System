/*
 * Buzzer.h
 *
 *  Created on: Mar 19, 2024
 *      Author: dell
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/****BUZZER PINS****/
#define BUZZER_PORT PORTC_ID
#define BUZZER_PIN PIN2_ID
/********************/

/**function prototypes**/
/*
 * Description
⮚ Setup the direction for the buzzer pin as output pin through the GPIO driver.
⮚ Turn off the buzzer through the GPIO.
 Inputs: None
 Return: None*/
void Buzzer_init();


/*
 * Description
⮚ Function to enable the Buzzer through the GPIO.
 Inputs: None
 Return: None*/
void Buzzer_on(void);


/*
 * Description
⮚ Function to disable the Buzzer through the GPIO.
 Inputs: None
 Return: No*/
void Buzzer_off(void);

#endif /* BUZZER_H_ */
