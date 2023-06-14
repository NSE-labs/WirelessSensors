/*
 * Driver header file for the SX1262 LoRa transceiver
 * Copyright 2023 NSE Labs, LLC
 * This software is free to use
 */
#ifndef SX1262
#define SX1262
#include "stm32l1xx_hal.h"

HAL_StatusTypeDef LoRa_SPI_Init(void);

/* SX1262 SPI opcodes */
#define LORA_GET_STATUS 0xC0;

/* Function definitions */
HAL_StatusTypeDef LoRa_SPI_Init(void);
uint8_t LoRaGetStatus(); /* returns the status byte from the SX1262 chip */

#endif
