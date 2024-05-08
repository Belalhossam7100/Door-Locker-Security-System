/*
 * uart_mine.h
 *
 *  Created on: Mar 2, 2024
 *      Author: dell
 */

#ifndef UART_MINE_H_
#define UART_MINE_H_

#include "std_types.h"



						/**CONFIGUARTION**/
typedef enum{
five_bit=0,six_bit=1,seven_bit=2,eight_bit=3
}UART_BitData;

typedef enum{
disable=0,reserved=1,even_parity=2,odd_parity=3
}UART_Parity;


typedef enum{
one_Bit=0,two_Bit=1
}UART_StopBit;


typedef enum {
    baud_2400   = 2400,
    baud_4800   = 4800,
    baud_9600   = 9600,
	baud_14400  = 14400,
    baud_19200  = 19200,
    baud_38400  = 38400,
    baud_57600  = 57600,
    baud_115200 = 115200,
    baud_128000 = 128000,
    baud_256000 = 256000,
} UART_BaudRate;


typedef struct{
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
	UART_BaudRate baud_rate;
}UART_ConfigType;
/**used UART configuration**/
UART_ConfigType uartConfig;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);
#endif /* UART_MINE_H_ */
