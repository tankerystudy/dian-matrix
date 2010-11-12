#ifndef __IIC_DRIVER_H__
#define __IIC_DRIVER_H__
////////////////////////////////////////////////////////////
// the I2C communication package, implement like file operations.
#include "common_def.h"

static byte const IIC_OVERTIME = 100;

enum {
    I2CDEFAULT= 0x00,
    I2CREAD= 0x01,      // 0000 0001b
    I2CPAGE1= 0x02      // 0000 0010b
};

// open EEPROM at memAddr with specific mode
// default mode is: WRITE, PAGE0, ADDR00
// using | to change the mode
// return if open success
bit I2COpen(const byte memAddr, const byte mode);
// write specific length byte from dataAddr
// returns:
//      0: write succes
//      1: write byte failed
//      2: page write overtime
//      3: insufficient memory space (including the PAGE setting
//          (A2A1P0) during Device Addressing)
byte I2CWrite(const byte* dataAddr, const byte length);
// read specific length byte to dataAddr just one time
void I2CReadOneTime(byte* dataAddr, const byte length);
// close EEPROM
void I2CClose(void);
// reset EEPROM
void I2CReset(void);



#endif // #ifndef SEED51_C_I2C_COM_H
