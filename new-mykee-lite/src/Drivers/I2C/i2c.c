#include "lpc23xx.h"
#include "i2c.h"
#include "menu_main.h"
#include "ticker_itrp.h"

#ifndef NULL
#define NULL 0
#endif


#define SCHL 60    /*I2C SPEED(KHz)=PCLK(MHz)/(SCHL+SCLL)  100KHz= 48000000/(120*4)*/
#define SCLL 60

static volatile u8_t  block_value[16];
static u8_t i2c_value[24];


static void i2c_lpc_wr_byte(int byte)
{

   I21DAT = byte;
   I21CONCLR = I2C_FLAG_SI;                //-- Clear SI
   while(!(I21CONSET & I2C_FLAG_SI));      //-- End wr POINT
}

//---------------------------------------------------------------------------
static void i2c_lpc_stop()
{
    //-- Set STOP condition
   I21CONCLR = I2C_FLAG_SI;                  //-- Clear SI
   I21CONSET |=  I2C_FLAG_AA | I2C_FLAG_STO; //-- Clear NO ASK
}
//---------------------------------------------------------------------------
static int i2c_lpc_ctrl(int ctrl)
{
   int chk;
   //-- Set START
   I21CONCLR = 0xFF; // Clear all bits
   I21CONSET |= I2C_FLAG_I2EN | I2C_FLAG_STA;
   while(!(I21CONSET & I2C_FLAG_SI));      //--- End START
   //-- Set ADDRESS
   I21DAT = ctrl;
   I21CONCLR = I2C_FLAG_STA | I2C_FLAG_SI; //-- Clear START & SI
   if(ctrl & 1) //-- RD
      chk = 0x40; //-- 40H - SLA+R has been transmitted; ACK has been received
   else
      chk = 0x18; //-- 18H - SLA+W has been transmitted; ACK has been received
   while(!(I21CONSET & I2C_FLAG_SI));      //-- End CTRL
   if(I21STAT != chk)
   {
      i2c_lpc_stop();
      return I2C_ERR_NO_RESPONSE;
   }
   return I2C_NO_ERR;
}

static int i2c_lpc_rx_to_buf()
{
   int rc,i;
   char val;
   char szstr[32]="";
   u8_t count_flag=0;
   int num=5;

   rc = num;
   if(rc > 1)
   {
      I21CONCLR = I2C_FLAG_SI;
      I21CONSET |= I2C_FLAG_AA;
      for(;;)
      {
         while(!(I21CONSET & I2C_FLAG_SI));  //-- End Data from slave;
         val= (unsigned char)I21DAT;
		 if(!count_flag)
		 {
			rc=num=val+1;
			count_flag++;
		 }
		 //initially rc=5 & num=5
		 //rc getting increase in each time.So szstr[value getting increase]
		 //upto 5 bytes will be store in  total loop
		 
		 szstr[num-rc]=val;
         rc--;
         if(rc <= 0)
            break;
         else if(rc == 1)
            I21CONCLR = I2C_FLAG_AA | I2C_FLAG_SI;  //-- After next will NO ASK
         else
         {
            I21CONCLR = I2C_FLAG_SI;
            I21CONSET |= I2C_FLAG_AA;
         }
      }
   }
   else if(rc == 1)
   {
      I21CONCLR = I2C_FLAG_AA | I2C_FLAG_SI;  //-- After next will NO ASK
      while(!(I21CONSET & I2C_FLAG_SI));  //-- End Data from slave;
      val= (unsigned char)I21DAT;
   }
   else //err
      return I2C_ERR_WRONG_PARAM;
   
   for(i=0;i<num;i++)
   {	   
		i2c_value[i]=szstr[i];
   }
   
   return I2C_NO_ERR;
}


//----------------------------------------------------------------------
static int i2c_lpc_ask_polling_op(int ctrl)  //-- wait until write finished
{
   int rc;
   int i;

   for(i=0;i < I2C_WR_24XX_TIMEOUT; i++) //-- actually wr = ~110 but timeout =10000
   {
      I21CONSET = I2C_FLAG_STA;
      I21CONCLR = I2C_FLAG_SI;  //-- Here - clear only SI (not all LPC_I2C1->I2CONCLR)
      while(!(I21CONSET & I2C_FLAG_SI));        //wait the ACK

      I21DAT = ctrl & 0xFE;; // R/WI = 0
      I21CONCLR = I2C_FLAG_SI | I2C_FLAG_STA; //-- Clear START & SI
      while(!(I21CONSET & I2C_FLAG_SI));//wait the ACK
      rc = I21STAT;
      if(rc == 0x18) //-- got ACK after CLA + W
         break;
      else
      {
      }
   }
   if(i == I2C_WR_24XX_TIMEOUT)
      return I2C_ERR_24XX_WR_TIMEOUT;
   return I2C_NO_ERR;
}

u8_t send_i2c_cmd() 
{
	u8_t i;
	if(i2c_lpc_ctrl(0xA0)!= I2C_NO_ERR) //-- Now WR (RD/WI = 0)
	{
		return 0;
	}
	for(i=0;i<=block_value[0];i++)
	{
		i2c_lpc_wr_byte(block_value[i]);
	}
	i2c_lpc_stop();		
	if(i2c_lpc_ask_polling_op(0xA0)!= I2C_NO_ERR)	  //-- wait until write finished
	{
		return 0;
	}
	i2c_lpc_stop();
	if(block_value[1]==2)
		delay(200);
	else
		delay(100);
	if(i2c_lpc_ctrl(0xA1)!= I2C_NO_ERR)
	{
		return 0;
	}
	if(i2c_lpc_rx_to_buf() != I2C_NO_ERR)
	{
		return 0;
	}
	i2c_lpc_stop();
	return 1;
}

void i2c_lpc_init(int Mode)
{
		u32_t pclksel;
    PINSEL0 &= ~0x0000000F;
    PINSEL0 |=  0x0000000F;	
 

	 pclksel= (PCLKSEL1 & (~(0x3<<6))) | (0x01 << 6);// Enable Timer 0 clock full speed (48 MHz)
	 PCLKSEL1 = pclksel;

   if(Mode == I2C_SPEED_400)
   {
  
      I21SCLH = SCHL;
      I21SCLL = SCLL;
   }
   else //Slow
   {
      I21SCLH = SCHL*4;
      I21SCLL = SCLL*4;
   }

   I21CONCLR = 0xFF;           //-- Clear all flags
   I21CONSET = 0x40;           //-- Set Master Mode
   I21CONSET |= I2C_FLAG_I2EN; //--- Enable I2C
}

u8_t* get_block_value(void)
{
	return block_value;
}

u8_t* get_i2c_value(void)
{
	return i2c_value;
}
/*-----------------------------------------end of file-------------------------------------------*/


