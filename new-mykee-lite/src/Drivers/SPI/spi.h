#ifndef __SPI_H__
#define __SPI_H__

#include "ticker_itrp.h"

#define LOW             0
#define HIGH            1


#define F_CLK    (1 <<  15)		//p0.15	 output
#define F_MOSI   (1 <<  18)		//p0.18	 output
#define F_CS     (1 <<  16)		//p0.16	 output
#define F_MISO   (1 <<  17)		//p0.17	 input


unsigned char spi_byte(unsigned char val);
void spi_cs(u8_t op);

#endif /* End of define __M25P16_H__ */

