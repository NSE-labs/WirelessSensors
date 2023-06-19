/*
 * Driver header file for the SX1262 LoRa transceiver
 * Copyright 2023 NSE Labs, LLC
 * This software is free to use
 */
#ifndef SX1262
#define SX1262
#include "stm32l1xx_hal.h"

typedef enum
{
  LoRa_OK       = 0x00U,
  LoRa_ERROR    = 0x01U,
  LoRa_TIMEOUT 	= 0x02U
} LoRa_Status;

/* SX1262 SPI commands */
#define LORA_GET_STATUS_OPCODE 			0xC0;

#define LORA_SET_STANDBY_OPCODE 		0x80;
#define LORA_STDBY_RC 					0;
#define LORA_STDBY_XOSC 				1;

#define LORA_SET_PACKET_TYPE_OPCODE 	0x8A;
#define LORA_PACKET_TYPE_LORA			1;

#define LORA_SET_RF_FREQUENCY_OPCODE	0x86;

#define LORA_SET_PA_CONFIG_OPCODE		0x95;

#define LORA_SET_TX_PARAMS_OPCODE		0x8E;

#define LORA_SET_DIO2_AS_RF_SWITCH_CTRL	0x9D;

#define LORA_SET_TX_CONTINUOUS_WAVE		0xD1;


/* Function definitions */
LoRa_Status LoRa_Init(SPI_HandleTypeDef *);
uint8_t LoRaGetStatus(void);
LoRa_Status LoRaSetTxContinuousWave(void);
LoRa_Status LoRaSetStandbyMode(void);

#endif
