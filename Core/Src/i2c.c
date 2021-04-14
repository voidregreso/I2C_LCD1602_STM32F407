/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
#include <stdio.h>

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */
	// Pull up the level of I2C_SCL and I2C_SDA
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void I2C_Start() {
	SDA_SETOUT();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_Delay(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // SDA
	HAL_Delay(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
}

void I2C_Stop() {
	SDA_SETOUT();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_Delay(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); // SCL
	HAL_Delay(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // SDA
}

uint8_t I2C_Esperar() {
	uint8_t timeout = 0;
	SDA_SETIN();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // SDA
	HAL_Delay(1);
	while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET) { // SDA in
		timeout++;
		if(timeout>250) {
			I2C_Stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
	return 0;
}

void I2C_Ack() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
	SDA_SETOUT();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // SDA
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); // SCL
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
}

void I2C_NAck() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
	SDA_SETOUT();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // SDA
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); // SCL
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
}

void I2C_SendByte(uint8_t txd) {
	uint8_t t, cur;
	SDA_SETOUT();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
	for(t=0;t<8;t++) {
		cur = (txd & 0x80) >> 7;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, cur); // SDA
		txd <<= 1;
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); // SCL
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
		HAL_Delay(2);
	}
}

uint8_t I2C_ReadByte(int isack) {
	uint8_t i, recv = 0;
	SDA_SETIN();
	for(i=0;i<8;i++) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); // SCL
		recv <<= 1;
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET) recv++;
		HAL_Delay(1);
	}
	if(!isack) I2C_NAck();
	else I2C_Ack();
	return recv;
}

///////////////
/******** Part of 24CXX ********/

uint8_t AT24CXX_ReadOneByte(uint16_t addr) {
	uint8_t tmp = 0;
	I2C_Start();
	I2C_SendByte(0XA0+((addr/256)<<1)); //Send device address 0XA0, write data
	I2C_Esperar();
	I2C_SendByte(addr%256); //Send low address
	I2C_Esperar();
	I2C_Start();
	I2C_SendByte(0XA1); //Enter receive mode
	I2C_Esperar();
	tmp = I2C_ReadByte(0);
	I2C_Stop();//Generate a stop condition
	return tmp;
}

void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t	DataToWrite) {
	I2C_Start();
	I2C_SendByte(0XA0+((WriteAddr/256)<<1));
	I2C_Esperar();
	I2C_SendByte(WriteAddr%256);
	I2C_Esperar();
	I2C_SendByte(DataToWrite);
	I2C_Esperar();
	I2C_Stop();
	HAL_Delay(10); //The EEPROM writing process is relatively slow, so you need to wait for a while, and then write again
}

void AT24CXX_WriteLenByte(uint16_t addr, uint32_t dato, uint8_t len) {
	uint8_t t;
	for(t=0;t<len;t++) {
		AT24CXX_WriteOneByte(addr+t, (dato>>(8*t))&(0xff));
	}
}

uint32_t AT24CXX_ReadLenByte(uint16_t addr, uint8_t len) {
	uint8_t t;
	uint32_t res = 0;
	for(t=0;t<len;t++) {
		res <<= 8;
		res += AT24CXX_ReadOneByte(addr+len-t-1);
	}
	return res;
}

uint8_t AT24CXX_Check() {
	uint8_t res = AT24CXX_ReadOneByte(255);
	if(res == 0x55) return 0;
	else { // Eliminate the circumstance of the first initialization
		AT24CXX_WriteOneByte(255,0x55);
		res = AT24CXX_ReadOneByte(255);
		if(res == 0x55) return 0;
		else printf("Error receiving the code %x\r\n", res);
	}
	return 1;
}

void AT24CXX_Read(uint16_t addr, uint8_t *buffer, uint16_t numeros) {
	while(numeros) {
		*buffer++ = AT24CXX_ReadOneByte(addr++);
		numeros--;
	}
}

void AT24CXX_Write(uint16_t addr, uint8_t *buffer, uint16_t numeros) {
	while(numeros--) {
		AT24CXX_WriteOneByte(addr, *buffer);
		addr++;
		buffer++;
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
