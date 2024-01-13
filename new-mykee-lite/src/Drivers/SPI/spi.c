#include "LPC23xx.h"
#include "SPI.h"
#include "menu_main.h"


unsigned char spi_byte(unsigned char val)
{
    unsigned char i, in_val, out_val;
    unsigned int rd;

    in_val = 0;
    out_val = val;

    for (i = 0; i < 8; i++)
    {
        in_val <<= 1;
        
        if (out_val & 0x80)
        {
            FIO0SET = F_MOSI;
            FIO0SET = F_MOSI;
            FIO0SET = F_MOSI;
            FIO0SET = F_MOSI;
        }
        else
        {
            FIO0CLR = F_MOSI;
            FIO0CLR = F_MOSI;
            FIO0CLR = F_MOSI;
            FIO0CLR = F_MOSI;
        }

        out_val <<= 1;
        
        rd = FIO0PIN;
        if (rd & F_MISO)
            in_val |= 1;

        FIO0SET = F_CLK;
        FIO0SET = F_CLK;
        FIO0SET = F_CLK;
        FIO0SET = F_CLK;
        
        FIO0CLR = F_CLK;
        FIO0CLR = F_CLK;
        FIO0CLR = F_CLK;
        FIO0CLR = F_CLK;
    }

    return(in_val);
}

void spi_cs(u8_t op)
{
    if (op == LOW)
    {
        FIO0CLR = F_CS;
        FIO0CLR = F_CS;
        FIO0CLR = F_CS;
        FIO0CLR = F_CS;
    }
    else
    {
        FIO0SET = F_CS;
        FIO0SET = F_CS;
        FIO0SET = F_CS;
        FIO0SET = F_CS;
    }
}

