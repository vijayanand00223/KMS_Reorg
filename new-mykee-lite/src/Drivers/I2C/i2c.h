#ifndef _I2C_H_
#define _I2C_H_

#include "ticker_itrp.h"

//---  LPC21XX I2C flags --------------------

#define  I2C_FLAG_AA    (1<<2)
#define  I2C_FLAG_SI    (1<<3)
#define  I2C_FLAG_STO   (1<<4)
#define  I2C_FLAG_STA   (1<<5)
#define  I2C_FLAG_I2EN  (1<<6)

//---- I2C Speed
#define  I2C_SPEED_100   0
#define  I2C_SPEED_400   1

//--- Errors

#define  I2C_NO_ERR                    0
#define  I2C_ERR_NO_RESPONSE           1
#define  I2C_ERR_WRONG_PARAM           2
#define  I2C_ERR_24XX_WR_TIMEOUT       3
#define  I2C_ERR_24XX_BAD_ADDR         4
#define  I2C_ERR_24XX_BAD_TYPE         5
#define  I2C_ERR_24XX_BAD_PAGESIZE     6
#define  I2C_ERR_24XX_WRONG_NUM        7
#define  I2C_ERR_LM75_WRONG_REGISTER   8

#define  I2C_WR_24XX_TIMEOUT     10000


//---- Prototypes ---

void i2c_lpc_init(int Mode);

extern void i2c_test1(void);
u8_t* get_block_value(void);
u8_t*  get_i2c_value(void);


#endif



