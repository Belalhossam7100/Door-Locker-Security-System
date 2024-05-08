/*
 * I2C_mine.h
 *
 *  Created on: Mar 16, 2024
 *      Author: dell
 */

#ifndef I2C_MINE_H_
#define I2C_MINE_H_
#include "std_types.h"

/*******************************************************************************
 *                      I2C Configuration                                    *
 *******************************************************************************/
#define TWPS 0x00 //pre_scaler
typedef uint8 TWI_Address;//device address

typedef enum{
	TWI_BaudRate_100_KB_persec
	,TWI_BaudRate_400_KB_persec
	,TWI_BaudRate_1_MB_persec
	,TWI_BaudRate_3_point_four_MB_persec
}TWI_BaudRate;

typedef struct{
TWI_Address address;//TWAR
TWI_BaudRate bit_rate;//TWBR
}TWI_ConfigType;

/**used I2C configuration**/
TWI_ConfigType twiConfig;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(const TWI_ConfigType * Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);




#endif /* I2C_MINE_H_ */
