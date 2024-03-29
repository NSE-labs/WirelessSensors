/*
 * Driver header file for the SX1262 LoRa transceiver
 * Copyright 2023 NSE Labs, LLC
 *
 * Adapted for use with the STM32WLE series of processors with the SX1262 on board
 *
 *
 * This software is free to use
 */
#ifndef SX1262
#define SX1262
#include "stm32wlxx_hal.h"

typedef enum
{
  LoRa_OK       = 0x00U,
  LoRa_ERROR    = 0x01U,
  LoRa_TIMEOUT 	= 0x02U
} LoRa_Status;

/* SX1262 SPI commands */
#define LORA_GET_STATUS_OPCODE 					0xC0;

#define LORA_SET_STANDBY_OPCODE 				0x80;
#define LORA_STDBY_RC 							0;
#define LORA_STDBY_XOSC 						1;

#define LORA_SET_PACKET_TYPE_OPCODE 			0x8A;
#define LORA_PACKET_TYPE_LORA					1;

#define LORA_SET_RF_FREQUENCY_OPCODE			0x86;

#define LORA_SET_PA_CONFIG_OPCODE				0x95;

#define LORA_SET_TX_PARAMS_OPCODE				0x8E;
#define LORA_STANDARD_RAMPTIME					0x04; /* 200mS ramp time */

#define LORA_SET_DIO2_AS_RF_SWITCH_CTRL_OPCODE	0x9D;

#define LORA_SET_TX_CONTINUOUS_WAVE_OPCODE		0xD1;

#define LORA_SET_REGULATOR_MODE_OPCODE			0x96;

#define LORA_SET_BUFFER_BASE_ADDRESS_OPCODE		0x8F;

#define LORA_SET_MODULATION_PARAMS_OPCODE		0x8B;

#define LORA_SET_PACKET_PARAMS_OPCODE			0x8C;

#define LORA_WRITE_BUFFER_OPCODE				0x0E;

#define LORA_SET_TX_OPCODE						0x83;

#define LORA_SET_RX_OPCODE						0x82;

#define LORA_SET_DIO_IRQ_PARAMS_OPCODE			0x08;

#define LORA_CLEAR_IRQ_STATUS_OPCODE			0X02;

#define LORA_GET_RX_BUFFER_STATUS_OPCODE		0x13;

#define LORA_READ_BUFFER_OPCODE					0x1E;


/* Function definitions */
LoRa_Status LoRa_Init(SUBGHZ_HandleTypeDef *);
uint8_t LoRaGetStatus(void);
LoRa_Status LoRaSetTxContinuousWave(void);
LoRa_Status LoRaSetStandbyMode(void);
LoRa_Status LoRaSetTxPower(int8_t);
LoRa_Status LoRaTransmit(uint8_t *, uint8_t);
LoRa_Status LoRaReceive(uint8_t *, uint32_t);



#endif
