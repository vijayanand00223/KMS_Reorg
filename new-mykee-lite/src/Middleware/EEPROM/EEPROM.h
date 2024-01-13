#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "ticker_itrp.h"

#define LOW             0
#define HIGH            1

#define SECTOR0         0x000000
#define SECTOR1         0x010000
#define LABEL_ADDR      SECTOR0

#define    WRITE_EN         0x06
#define    WRITE_DISABLE    0x04
#define    READ_ID          0x9f
#define    READ_STATUS_REG  0x05
#define    WRITE_STATUS_REG 0x01
#define    READ_DATA        0x03
#define    FAST_READ_DATA   0x0b
#define    PAGE_PRO         0x02
#define    SECTOR_ERASE     0xd8
#define    BULK_ERASE       0xc7
#define    M25P16_RES       0xab
#define    POWER_DOWN       0xb9

void WriteEn (u8_t op);
u8_t M25p16Reset(void);
void M25p16PowerOff(void);
u8_t ReadByte(u32_t address);
void WriteByte(u32_t address, u8_t data);
void ReadBytes(u32_t address, u8_t *block, u16_t count);
u8_t WrBytes (u32_t address, u8_t *block, u16_t count);
void M25p16Erase(void);
void SectorErase(u32_t address);
u8_t ReadStaReg(void);
void WrStaReg(u8_t data);
void ReadID(u8_t *data);

#endif

