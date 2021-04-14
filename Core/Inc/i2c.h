/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

//Clear bit, and then setbit!
#define SDA_SETIN() { \
	GPIOB->MODER &= ~(3<<(9*2)); \
	GPIOB->MODER |= 0;\
}

#define SDA_SETOUT() { \
	GPIOB->MODER &= ~(3<<(9*2)); \
	GPIOB->MODER |= 1<<(9*2);\
}

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);

void I2C_Start();
void I2C_Stop();
uint8_t I2C_Esperar();
void I2C_Ack(); // acknowledge/reponse of I2C ACK
void I2C_NAck(); // do not produce ack response
void I2C_SendByte(uint8_t txd);
uint8_t I2C_ReadByte(int isack);
uint8_t AT24CXX_ReadOneByte(uint16_t addr);
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t	DataToWrite);
void AT24CXX_WriteLenByte(uint16_t addr, uint32_t dato, uint8_t len);
uint32_t AT24CXX_ReadLenByte(uint16_t addr, uint8_t len);
uint8_t AT24CXX_Check();
void AT24CXX_Read(uint16_t addr, uint8_t *buffer, uint16_t numeros);
void AT24CXX_Write(uint16_t addr, uint8_t *buffer, uint16_t numeros);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
