#include <reg52.h>
#include "lic_driver.h"


static byte currentAddress;
static byte currentMode;


extern bit ASMI2COpen(byte);
extern bit ASMI2CWriteByte(byte);
extern void ASMI2CClose(void);
extern byte ASMI2CReadByte(void);
extern void ASMI2CSendACK(byte);
extern void ASMI2CReset(void);

// open EEPROM at memAddr with specific mode
// default mode is: WRITE, PAGE0, ADDR00
// using | to change the mode
// return if open succes
bit I2COpen(const byte memAddr, const byte mode)
{
    currentMode = mode | 0xA0;              // add memorizer mode
    currentAddress = memAddr;

    if (!ASMI2COpen(0xfe & currentMode))    // force using write mode
        return 0;
    if (!ASMI2CWriteByte(currentAddress))   // write addr to move here
        return 0;
    if ((currentMode & 0x01) == I2CREAD)    // if is read, reopen as read
        if (!ASMI2COpen(currentMode))
            return 0;

    return 1;    // reopen
}

// write specific length byte from dataAddr
// returns:
//      0: write succes
//      1: write byte failed
//      2: page write will overflow
//      3: insufficient memory space (including the PAGE setting
//          (A2A1P0) during Device Addressing)
byte I2CWrite(const byte* dataAddr, const byte length)
{
    byte i;

    if ((~currentAddress & 0x0f)+1 < length)    // low 4 bit overflow
    {
        if ((~currentAddress & 0xff)+1 < length)       // whole address overflow
            return 3;       // insufficient memory space
        else
            return 2;       // page write will overflow
    }
    for (i = 0; i < length; i++)    // Sequential Write
    {
        if (!ASMI2CWriteByte(*(dataAddr + i)))
            return 1;
        currentAddress++;
    }

    return 0;   // normal
}

// read specific length byte to dataAddr just one time
void I2CReadOneTime(byte* dataAddr, const byte length)
{
    byte i;
    for (i = 0; i < length - 1; i++)    // Sequential Read
    {
        *(dataAddr+i) = ASMI2CReadByte();
        ASMI2CSendACK(1);       // Send ACK
    }
    *(dataAddr+i) = ASMI2CReadByte();
    ASMI2CSendACK(0);           // Send NOACK
}

// close EEPROM
void I2CClose(void)
{
    ASMI2CClose();
}

// reset EEPROM
void I2CReset()
{
    ASMI2CReset();
}

