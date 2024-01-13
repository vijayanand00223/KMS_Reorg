#include "LPC23xx.h"
#include "EEPROM.h"
#include "SPI.h"
#include "menu_main.h"

void WriteEn (u8_t op)
{
    spi_cs(LOW);
    if (op == HIGH)
    {
        spi_byte(WRITE_EN);
    }
    else
    {
        spi_byte(WRITE_DISABLE);
    }
    spi_cs(HIGH);
}

u8_t M25p16Reset(void)
{
    u8_t data;
    spi_cs(0);
    spi_byte(M25P16_RES);
    spi_byte(0);
    spi_byte(0);
    spi_byte(0);
    data = spi_byte(0xFF);
    spi_cs(HIGH);
    return(data);
}

void M25p16PowerOff(void)
{
    spi_cs(LOW);
    spi_byte(POWER_DOWN);
    spi_cs(HIGH);
}

u8_t ReadByte(u32_t address)
{
    u8_t data = 0;
    spi_cs(LOW);
    spi_byte(READ_DATA);
    spi_byte((address & 0x00ff0000)>>16);
    spi_byte((address & 0x0000ff00)>>8);
    spi_byte(address & 0x000000ff);
    data = spi_byte(0xFF);
    spi_cs(HIGH);
    return(data);
}

void WriteByte(u32_t address, u8_t data)
{
    u8_t IsOver = 0;
    WriteEn(HIGH);
    spi_cs(LOW);
    spi_byte(PAGE_PRO);
    spi_byte((address & 0x00ff0000)>>16);
    spi_byte((address & 0x0000ff00)>>8);
    spi_byte(address & 0x000000ff);
    spi_byte(data);
    spi_cs(HIGH);
    do
    {
        spi_cs(LOW);
        spi_byte(READ_STATUS_REG);
        IsOver = spi_byte(0xFF);
        spi_cs(HIGH);
    }
    while (IsOver & 0x01);
}

void ReadBytes(u32_t address, u8_t *block, u16_t count)
{
    spi_cs(LOW);
    spi_byte(FAST_READ_DATA);
    spi_byte((address & 0x00ff0000)>>16);
    spi_byte((address & 0x0000ff00)>>8);
    spi_byte(address & 0x000000ff);
    spi_byte(0x00);                          //dummy byte
    while (count)
    {
        *block = spi_byte(0xFF);
        block++;
        count--;
    }
    spi_cs(HIGH);
}

u8_t WrBytes (u32_t address, u8_t *block, u16_t count)
{
    u8_t IsOver = 0;
    if (((address & 0xff) + count - 1) <= 0xff)
    {
        WriteEn(HIGH);
        spi_cs(LOW);
        spi_byte(PAGE_PRO);
        spi_byte((address & 0x00ff0000)>>16);
        spi_byte((address & 0x0000ff00)>>8);
        spi_byte(address & 0x000000ff);
        spi_byte(*block);
        count--;
        block++;
        address++;
        while (count > 0 && ((address & 0xff) != 0))
        {
            spi_byte(*block);
            count--;
            address++;
            block++;
        }
        spi_cs(HIGH);
	    do
	    {
            spi_cs(LOW);
		    spi_byte(READ_STATUS_REG);
		    IsOver = spi_byte(0x00);
            spi_cs(HIGH);
	    }
    	while (IsOver & 0x01);
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

void M25p16Erase(void)
{
    u8_t IsOver = 0;
    WriteEn(HIGH);
    spi_cs(LOW);
    spi_byte(BULK_ERASE);
    spi_cs(HIGH);
    do
    {
        spi_cs(LOW);
        spi_byte(READ_STATUS_REG);
        IsOver = spi_byte(0xFF);
        spi_cs(HIGH);
    }
    while (IsOver & 0x01);
}

void SectorErase(u32_t address)
{
    u8_t IsOver = 0;
		if(!address && !sys_erase_flg)
			return;
    WriteEn(HIGH);
    spi_cs(LOW);
    spi_byte(SECTOR_ERASE);
    spi_byte((address & 0x00ff0000)>>16);
    spi_byte((address & 0x0000ff00)>>8);
    spi_byte(address & 0x000000ff);
    spi_cs(HIGH);
    do
    {
        spi_cs(LOW);
        spi_byte(READ_STATUS_REG);
        IsOver = spi_byte(0xFF);
        spi_cs(HIGH);
    }
    while (IsOver & 0x01);
		sys_erase_flg=0;
}

u8_t ReadStaReg(void)
{
    u8_t data = 0;
    spi_cs(LOW);
    spi_byte(READ_STATUS_REG);
    data = spi_byte(0xFF);
    spi_cs(HIGH);
    return(data);
}

void WrStaReg(u8_t data)
{
    WriteEn(HIGH);
    spi_cs(LOW);
    spi_byte(WRITE_STATUS_REG);
    spi_byte(data);
    spi_cs(HIGH);
}

void ReadID(u8_t *data)
{
    int i;
    
    spi_cs(LOW);
    
    spi_byte(READ_ID);

    for (i = 0; i < 20; i++)
    {
        *data = spi_byte(0xFF);
        data++;
    }

    spi_cs(HIGH);
}


