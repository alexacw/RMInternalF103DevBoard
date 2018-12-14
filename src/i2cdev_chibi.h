// I2Cdev library collection - Main I2C device class header file
// Abstracts bit and byte I2C R/W functions into a convenient class
// 6/9/2012 by Jeff Rowberg <jeff@rowberg.net>
//
// Changelog:
//  2018-11-26 - fix hardware issue of stm32f1 MCUs that I2C transmit via DMA need 2 or more bytes to work
//  2012-06-09 - fix major issue with reading > 32 bytes at a time with Arduino Wire
//             - add compiler warnings when using outdated or IDE or limited I2Cdev implementation
//  2011-11-01 - fix write*Bits mask calculation (thanks sasquatch @ Arduino forums)
//  2011-10-03 - added automatic Arduino version detection for ease of use
//  2011-10-02 - added Gene Knight's NBWire TwoWire class implementation with small modifications
//  2011-08-31 - added support for Arduino 1.0 Wire library (methods are different from 0.x)
//  2011-08-03 - added optional timeout parameter to read* methods to easily change from default
//  2011-08-02 - added support for 16-bit registers
//             - fixed incorrect Doxygen comments on some methods
//             - added timeout value for read operations (thanks mem @ Arduino forums)
//  2011-07-30 - changed read/write function structures to return success or byte counts
//             - made all methods static for multi-device memory savings
//  2011-07-28 - initial release

/* ============================================
ChibiOS I2Cdev Main I2C device class code is placed under the MIT license
Copyright (c) 2012 Jan Schlemminger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _I2CDEV_CHIBI_H_
#define _I2CDEV_CHIBI_H_

#include "ch.h"
#include "hal.h"
#include <stdint.h>
#include <stdbool.h>
typedef bool bool_t;

#define I2CDEV_DEFAULT_READ_TIMEOUT 10
#define I2CDEV_BUFFER_LENGTH 64

int8_t I2CdevreadBit(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout);
int8_t I2CdevreadBitW(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout);
int8_t I2CdevreadBits(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout);
int8_t I2CdevreadBitsW(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint16_t timeout);
int8_t I2CdevreadByte(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout);
int8_t I2CdevreadWord(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout);
int8_t I2CdevreadBytes(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout);
int8_t I2CdevreadWords(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout);

bool_t I2CdevwriteBit(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
bool_t I2CdevwriteBitW(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
bool_t I2CdevwriteBits(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
bool_t I2CdevwriteBitsW(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
bool_t I2CdevwriteByte(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t data);
bool_t I2CdevwriteWord(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint16_t data);
bool_t I2CdevwriteBytes(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
bool_t I2CdevwriteWords(I2CDriver *driver, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

#endif /* _I2CDEV_CHIBI_H_ */
