/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

#include <inttypes.h>
#include "../mk-data-structs.h"

#define TWI_STATE_MOTOR_TX			0
#define TWI_STATE_MOTOR_RX			5
#define TWI_STATE_GYRO_OFFSET_TX	18

extern volatile uint8_t twi_state;
extern volatile uint8_t motor_write;
extern volatile uint8_t motor_read;
extern volatile uint8_t I2C_TransferActive;

extern uint8_t MissingMotor;

#define MAX_MOTORS						12
#define MOTOR_STATE_PRESENT_MASK		0x80
#define MOTOR_STATE_ERROR_MASK			0x7F
#define MOTOR_STATE_NEW_PROTOCOL_MASK	0x01
#define BLFLAG_TX_COMPLETE				0x01
#define BLFLAG_READ_VERSION				0x02

extern volatile uint8_t BLFlags;
extern char MotorenEin;
unsigned char MotorTest[16];
#define BL_READMODE_STATUS				0
#define BL_READMODE_CONFIG				16



extern MotorData_t Motor[MAX_MOTORS];

#define BLCONFIG_REVISION 2

#define MASK_SET_PWM_SCALING		0x01
#define MASK_SET_CURRENT_LIMIT		0x02
#define MASK_SET_TEMP_LIMIT			0x04
#define MASK_SET_CURRENT_SCALING	0x08
#define MASK_SET_BITCONFIG			0x10
#define MASK_RESET_CAPCOUNTER		0x20
#define MASK_SET_DEFAULT_PARAMS		0x40
#define MASK_SET_SAVE_EEPROM		0x80

#define BITCONF_REVERSE_ROTATION	0x01
#define BITCONF_RES1				0x02
#define BITCONF_RES2				0x04
#define BITCONF_RES3				0x08
#define BITCONF_RES4				0x10
#define BITCONF_RES5				0x20
#define BITCONF_RES6				0x40
#define BITCONF_RES7				0x80



extern BLConfig_t BLConfig;

extern volatile uint16_t I2CTimeout;

void I2C_Init(char); // Initialize I2C
#define I2C_Start(start_state)			{twi_state = start_state; BLFlags &= ~BLFLAG_TX_COMPLETE; TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT) | (1<<TWIE);}
#define I2C_Stop(start_state)			{twi_state = start_state; TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT);}
void I2C_Reset(void); // Reset I2C

#define BLCONFIG_SUCCESS				0
#define BLCONFIG_ERR_MOTOR_RUNNING		1
#define BLCONFIG_ERR_MOTOR_NOT_EXIST	2
#define BLCONFIG_ERR_HW_NOT_COMPATIBLE	3
#define BLCONFIG_ERR_SW_NOT_COMPATIBLE	4
#define BLCONFIG_ERR_CHECKSUM			5
#define BLCONFIG_ERR_READ_NOT_POSSIBLE	6

uint8_t I2C_WriteBLConfig(uint8_t motor);
uint8_t I2C_ReadBLConfig(uint8_t motor);

#endif


