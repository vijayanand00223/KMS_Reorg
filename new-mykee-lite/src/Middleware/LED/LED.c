#include "lpc23xx.h"
#include "i2c.h"
#include "menu_main.h"
#include "ticker_itrp.h"



u8_t red_led_onoff(u8_t flag)
{
	u8_t *block_value;u8_t *i2c_value;
	
	block_value=get_block_value();
	i2c_value=get_i2c_value();
	
	block_value[0]=0x02;
	block_value[1]=0x40;
	block_value[2]=flag;
	if(!send_i2c_cmd())
		return 0;
	if(i2c_value[2]==0x00)
		return 1;
	else
		return 0;
}

void greenled_onoff(u8_t flag)
{
	u8_t *block_value;
	block_value=get_block_value();
	
	block_value[0]=0x02;
	block_value[1]=0x41;
	block_value[2]=flag;
	send_i2c_cmd();
}
