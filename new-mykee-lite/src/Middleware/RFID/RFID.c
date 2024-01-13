#include "lpc23xx.h"
#include "RFID.h"
#include "i2c.h"
#include "menu_main.h"
#include "ticker_itrp.h"

const unsigned char select_mifarei2c[]={0x01,0x01};
u8_t regular_block_value[16];
volatile s32_t   card_id_available = 0; 
volatile u32_t card_id;

int regular_card(void)
{
	u8_t i;u8_t *block_value;u8_t *i2c_value;
	//char szstr[32];
	block_value= get_block_value();
	i2c_value= get_i2c_value();
	for(i=0;i<2;i++)
	{
			block_value[i]=select_mifarei2c[i];
	}
	if(!send_i2c_cmd())
		return 0;
		if(i2c_value[2]==0x00)
		{
			for(i=3;i<i2c_value[0];i++)
			{
				regular_block_value[i-3]=i2c_value[i];
			}
			
			card_id=0;
			card_id|=(regular_block_value[0] & 0xFF)|((regular_block_value[1]<<8)&0xFF00)|((regular_block_value[2]<<16)&0xFF0000)|((regular_block_value[3]<<24)&0xFF000000);
			card_id_available=0x01;
			return 1;
		}
		else if(i2c_value[2]==0x01)
		return 3;
	return 0;
}

