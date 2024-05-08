/*
 * uart_mine.c
 *
 *  Created on: Mar 2, 2024
 *      Author: dell
 */

#include"uart_mine.h"
#include"Common_Macros.h"
#include "avr/io.h" /* To use the UART Registers */

void UART_init(const UART_ConfigType *Config_Ptr){

	uint16 ubrr_value = 0;/*UBRR RGISTER INTIAL VALUE*/
	uint16 baud_rate_value=0;

	/* U2X = 1 for double transmission speed */
	SET_BIT(UCSRA,U2X);

	/************************** UCSRB Description **************************
		 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
		 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
		 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
		 * RXEN  = 1 Receiver Enable
		 * RXEN  = 1 Transmitter Enable
		 * UCSZ2 = 0 For 8-bit data mode
		 * RXB8 & TXB8 not used for 8-bit data mode
		 ***********************************************************************/
	SET_BIT(UCSRB,RXEN);
	SET_BIT(UCSRB,TXEN);

	/************************** UCSRC Description **************************
		 * URSEL   = 1 The URSEL must be one when writing the UCSRC
		 * UMSEL   = 0 Asynchronous Operation
		 * UPM1:0  = 00 Disable parity bit
		 * USBS    = 0 One stop bit
		 * UCSZ1:0 = 11 For 8-bit data mode
		 * UCPOL   = 0 Used with the Synchronous operation only
		 ***********************************************************************/
	SET_BIT(UCSRC,URSEL);/*The URSEL must be one when writing the UCSRC*/
	CLEAR_BIT(UCSRC,UMSEL);
/*Character size(bite mode)(bite size)*/
UCSRC=(UCSRC|0xF9)|((Config_Ptr->bit_data) &0x9F);
/*parity*/
UCSRC=(UCSRC|0xCF)|((Config_Ptr->parity) &0xFC);
/*stop bit*/
UCSRC=(UCSRC|0xF7)|((Config_Ptr->stop_bit) &0x7F);
CLEAR_BIT(UCSRC,UMSEL);


#ifdef nine_bit_mode
SET_BIT(UCSRB,UCSZ2);
SET_BIT(UCSRC,UCSZ1);
SET_BIT(UCSRC,UCSZ0);
#endif

	baud_rate_value=Config_Ptr->baud_rate;
	/* Calculate the UBRR register value */
	ubrr_value= (uint16)(((F_CPU / (baud_rate_value * 8UL))) - 1);
	//8UL: This represents the number of bits per UART frame, typically 8 bits per frame. The UL suffix stands for Unsigned Long.
		/*UL: The UL suffix denotes an unsigned long constant.
		 * In this context, it ensures that the calculation is performed using unsigned long integers.
		 *  This is important because it guarantees that the result will be large enough to hold the product of baud_rate * 8, especially if F_CPU is a large number and baud_rate is relatively small.
		 *  By specifying 8UL, the code ensures that the division operation (F_CPU / (baud_rate * 8UL)) is performed with enough precision to accurately calculate the required baud rate divisor.*/
	//(uint16): This casts the result to a 16-bit unsigned integer type. This is likely because the UBRR (USART Baud Rate Register) is typically a 16-bit register.


	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}


void UART_sendByte(const uint8 data){
	/*
		 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
		 * transmitting a new byte so wait until this flag is set to one
		 */
		while(BIT_IS_CLEAR(UCSRA,UDRE)){}

		/*
		 * Put the required data in the UDR register and it also clear the UDRE flag as
		 * the UDR register is not empty now
		 */
		UDR = data;
}
uint8 UART_recieveByte(void){
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
		while(BIT_IS_CLEAR(UCSRA,RXC)){}

		/*
		 * Read the received data from the Rx buffer (UDR)
		 * The RXC flag will be cleared after read the data
		 */
	    return UDR;
}
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

		/* Send the whole string */
		while(Str[i] != '\0')
		{
			UART_sendByte(Str[i]);
			i++;
		}
	}
/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	/* Receive the whole string until the '#' */
		while(Str[i] != '#')//'#'null ana 4mlha3lshan 2ra elstring
		{
			i++;
			Str[i] = UART_recieveByte();
		}

		/* After receiving the whole string plus the '#', replace the '#' with '\0' */
		Str[i] = '\0';
}


