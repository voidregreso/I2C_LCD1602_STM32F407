#ifndef __LCD_H__
#define __LCD_H__

#include <i2c.h>
#include "main.h"

void InitLCD();
void SendLCDCmd(uint8_t cmd);
void SendLCDData(uint8_t data);
void ClearLCD();
void WriteLCD(uint8_t x, uint8_t y, char *data);

#endif