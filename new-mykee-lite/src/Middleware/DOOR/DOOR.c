#include "lpc23xx.h"
#include "DOOR.h"
#include "menu_main.h"
#include "ticker_itrp.h"



/******************************************************************************
 ** Function name:      relay
 **
 ** Descriptions:       this function is used for switch ON/OFF the given two relays,
 **
 ** parameters:          num=> indicates which relay to access out of 2.
 **						 state=> indicates whether the relay should be ON/OFF
 ** Returned value:      None
 ** 
 ******************************************************************************/
void relay(s32_t num, s32_t state)
{
	s32_t irly;
if(sys_t.hooter_enable)
{
    if (num == 1)                                   // Relay 1 Port 2.8
    {
        irly = RLY1;
    }
    else if (num == 2)                              // relay 2 Port 2.9
    {
        irly = RLY2;
    }
    else
    {
        return;
    }
    if (state)
    {
        FIO2SET = irly;
    }
    else
    {
        FIO2CLR = irly;
    }
	}
}

/******************************************************************************
 ** Function name:      trg
 **
 ** Descriptions:       used for door open and close
 **
 ** parameters:          num
 ** Returned value:      iret
 ** 
 ******************************************************************************/

s32_t trg(s32_t num)
{
	s32_t iret = 0;

    switch (num)
    {
	    case 0:		iret = !(FIO4PIN & TRG3); 	break;			// inner door

	    case 1: 	iret = !(FIO0PIN & TRG1); 	break;			// outer door sensor
	
	    case 2: 
	    	iret = !(FIO4PIN & TRG2);///   (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x94))
	    	break;			//installer login
	    case 3: 
	    	iret = (FIO0PIN & TRG0); 		
	    	break;
			case 5:     iret =!(FIO0PIN & ISP);    break;                //   ISP=(1 << 10)
	    default:
	       break;
    }

    return (iret != 0);
}

//lock on and off
void lock_on(char a) 
{

	if(a==1)
	{
   FIO2SET=LOCKD;
  }
	if(a==0)
	{
	 FIO2CLR=LOCKD;
  }

}


