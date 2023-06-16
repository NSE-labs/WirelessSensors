/*
 * SX1262 LoRa transceiver driver
 * Copyright 2023 NSE Labs, LLC
 * Free to use
 */
#include <SX1262.h>
#include "main.h"

SPI_HandleTypeDef hspi;

HAL_StatusTypeDef LoRa_Init(void)
{
  HAL_StatusTypeDef retValue;

  /* SPI1 parameter configuration*/
  hspi.Instance = SPI1;
  hspi.Init.Mode = SPI_MODE_MASTER;
  hspi.Init.Direction = SPI_DIRECTION_2LINES;
  hspi.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial = 10;

  /* Initialize the SPI port */
  retValue = HAL_SPI_Init(&hspi);

  /* Take SX1262 chip out of reset */
  HAL_GPIO_WritePin(LoRa_NRST_GPIO_Port, LoRa_NRST_Pin, GPIO_PIN_SET);

  return(retValue);

}

uint8_t LoRaGetStatus(void)
{
	/* Return the status byte from the SX1262 chip */
	uint8_t status = 0;
	uint8_t opcode = LORA_GET_STATUS;

	if(HAL_SPI_Transmit(&hspi, &opcode, 1, 100) != HAL_OK)
	{
		return 0xFF;
	}

	if(HAL_SPI_Receive(&hspi, &status, 1, 100) != HAL_OK)
	{
			return 0xFF;
	}

	return status;
}
