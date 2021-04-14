#include <lcd.h>
#include <string.h>

void InitLCD() {
	SendLCDCmd(0x33);
	HAL_Delay(5);
	SendLCDCmd(0x32);
	HAL_Delay(5);
	SendLCDCmd(0x28);
	HAL_Delay(5);
	SendLCDCmd(0x0c);
	HAL_Delay(5);
	SendLCDCmd(0x01);
	HAL_Delay(5);
	AT24CXX_WriteOneByte(0, 0x08);
}

void SendLCDCmd(uint8_t cmd) {
	uint8_t tmp;
	tmp = cmd & 0xf0;
	tmp |= 0x04;
	AT24CXX_WriteOneByte(0, tmp);
	HAL_Delay(2);
	tmp &= 0xfb;
	HAL_Delay(2);
	
	tmp = (cmd & 0x0f) << 4;
	tmp |= 0x04;
	AT24CXX_WriteOneByte(0, tmp);
	HAL_Delay(2);
	tmp &= 0xfb;
	AT24CXX_WriteOneByte(0, tmp);
}

void SendLCDData(uint8_t data) {
	uint8_t tmp;
	tmp = data & 0xf0;
	tmp |= 0x05;
	AT24CXX_WriteOneByte(0, tmp);
	HAL_Delay(2);
	tmp &= 0xfb;
	AT24CXX_WriteOneByte(0, tmp);
	
	tmp = (data & 0x0f) << 4;
	tmp |= 0x05;
	AT24CXX_WriteOneByte(0, tmp);
	HAL_Delay(2);
	tmp &= 0xfb;
	AT24CXX_WriteOneByte(0, tmp);
}

void ClearLCD() {
	SendLCDCmd(0x01);
}

void WriteLCD(uint8_t x, uint8_t y, char *data) {
	uint8_t addr, i;
	if (x < 0)  x = 0;
	if (x > 15) x = 15;
	if (y < 0)  y = 0;
	if (y > 1)  y = 1;
	
	addr = 0x80 + 0x40 * y + x;
	SendLCDCmd(addr);
	
	for(i=0;i<strlen(data);i++) {
		SendLCDData(data[i]);
	}
}