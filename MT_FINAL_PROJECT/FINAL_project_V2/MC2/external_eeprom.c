 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "I2C_mine.h"

/*
 * Function: EEPROM_writeByte
 * Description: Writes a byte of data to the external EEPROM memory.
 * Parameters:
 *   - u16addr: a 16-bit unsigned integer representing the memory address where the data will be written.
 *   - u8data: an 8-bit unsigned integer representing the data to be written.
 * Returns:
 *   - An 8-bit unsigned integer (likely indicating success or failure).
 */

void EEPROM_init(){
	/*CONTROL ECU act as Master with device address 10
	 *  and the used baud rate should be 400K Bits/Sec.*/
	twiConfig.address = 0x10;
	twiConfig.bit_rate =TWI_BaudRate_400_KB_persec;
	TWI_init(&twiConfig);
}

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
    /*0xA0 = [1010 0000] , 0x0700=[0111 0000 0000]*/
    /*
     *
     *(u16addr & 0x0700) performs a bitwise AND operation between the memory address u16addr and the hexadecimal value 0x0700. This operation isolates the upper three bits of the memory address.
     *((u16addr & 0x0700) >> 7) shifts the result of the previous operation seven bits to the right. This effectively moves the upper three bits of the memory address to the least significant bits, preparing them to be combined with the device address.*/
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR; 
    /*
     * This line sends the device address over the TWI bus.
     * 	 The device address is constructed by combining 0xA0[1010 0000] with the upper bits of the memory address u16addr.
     *   The upper bits are obtained by masking the address with 0x0700 and then shifting right by 7 bits.
     *   TWI_writeByte() is a function used to write a byte over the TWI bus.*/
		 
    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));/**Sends the memory address to specify the location in the EEPROM where data will be written.*/
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* write byte to eeprom */
    TWI_writeByte(u8data);/** Writes the specified data byte to the specified memory address in the EEPROM*/
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
	
    return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));/**Sends the device address to initiate communication with the EEPROM.*/
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;
		
    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));/**ends the memory address to specify the location in the EEPROM where data will be read.*/
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));/**Sends the device address with the read bit set to initiate reading from the specified memory address.*/
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readByteWithNACK();/**Reads the specified data byte from the specified memory address in the EEPROM.*/
    /* By passing a pointer to a memory location rather than the actual value itself, the function can modify the content of that memory location directly.*/
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}
