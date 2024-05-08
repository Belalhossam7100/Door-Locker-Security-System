/*
 * I2C_MINE.c
 *
 *  Created on: Mar 16, 2024
 *      Author: dell
 */
#include "I2C_mine.h"
#include "common_macros.h"
#include <avr/io.h>

void TWI_init(const TWI_ConfigType * Config_Ptr)
{
	uint8 TWBR_value;

	switch(Config_Ptr->bit_rate)
	{
	case TWI_BaudRate_100_KB_persec:
		/*SCL freq=10kbs*/
		TWBR_value=(((F_CPU/100000)-16)/(2*pow(4,TWPS)));
		break;
	case TWI_BaudRate_400_KB_persec:
		/*SCL freq=40kbs*/
		TWBR_value=(((F_CPU/400000)-16)/(2*pow(4,TWPS)));
		break;
	case TWI_BaudRate_1_MB_persec:
		/*SCL freq=1mbs*/
		TWBR_value=(((F_CPU/1000000)-16)/(2*pow(4,TWPS)));
		break;
	case TWI_BaudRate_3_point_four_MB_persec:
		/*SCL freq=3.4mbs*/
		TWBR_value=(((F_CPU/3400000)-16)/(2*pow(4,TWPS)));
		break;
	}
	/*set pre_scaler*/
	TWBR=TWBR_value;
	TWSR = TWPS;
	// Set device address in TWAR register(clear TWGCE for MASTER)
	TWAR = ((Config_Ptr->address << 1)& ~TWGCE); // Device address left-shifted by 1


    TWCR = (1<<TWEN); /* enable TWI */
}


void TWI_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * 	 you want to clear it before initiating a new operation to ensure you're not starting with a stale flag.
	 *
	 * send the start bit by TWSTA=1
	 *   It's important to set it simultaneously with clearing the TWINT flag
	 *   to ensure that the start condition is sent immediately after the TWINT flag is cleared.
	 *
	 * Enable TWI Module TWEN=1
	 *   It should be set along with the other bits
	 *   to ensure that the TWI module is enabled at the same time as initiating the start condition.
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    /*
     * By setting all these bits together in a single operation, you ensure that there's no delay between clearing the TWINT flag,
     * initiating the start condition, and enabling the TWI module. This helps in maintaining the timing requirements of the I2C
     * protocol and ensures proper operation of the TWI hardware.
     */


    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data)
{
	/* Put data On TWI data Register */
	    TWDR = data;
	/*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	 TWCR = (1 << TWINT) | (1 << TWEN);
 /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	 while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}




