/*
 * SX1262 LoRa transceiver driver
 * Copyright 2023 NSE Labs, LLC
 * Free to use
 */
#include <SX1262.h>
#include "main.h"

SPI_HandleTypeDef *hspi;

/*
 * WaitWhileBusy
 * wait for the busy line on the SX1262 to go low indicating not busy
 * Arguments
 *  timeout in milliseconds (maximum time to wait for line to go low)
 * Return Value
 *  LoRa_OK for success
 *  LoRa_TIMEOUT for timeout
 */
static LoRa_Status WaitWhileBusy(uint16_t timeout)
{
	uint16_t timeoutCounter = 0;

	while(HAL_GPIO_ReadPin(LoRa_BUSY_GPIO_Port, LoRa_BUSY_Pin) == GPIO_PIN_SET)
	{
		timeoutCounter++;
		if(timeoutCounter > timeout)
		{
			return(LoRa_TIMEOUT);
		}
		HAL_Delay (1); /* Delay 1 mS */
	}

	return(LoRa_OK);
}


/*
 * LoRa_Init
 * Initialize the SX1262 chip
 * Arguments
 * 	Pointer to SPI_HandleTypeDef structure for the SPI port used to communicate with the SX1262
 * Return Value
 *  LoRa_OK for success
 *  LoRa_ERROR in case of error
 */
LoRa_Status LoRa_Init(SPI_HandleTypeDef *hspiPtr)
{
	uint8_t xmitBuffer[6];

  /* store the hspi pointer for future use */
  hspi = hspiPtr;

  /* Take SX1262 chip out of reset */
  HAL_GPIO_WritePin(LoRa_NRST_GPIO_Port, LoRa_NRST_Pin, GPIO_PIN_SET);

  /* Wait while the chip finishes its reset sequence */
  if(WaitWhileBusy(10) != LoRa_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the chip to Standby mode and using the external oscillator */
  xmitBuffer[0] = LORA_SET_STANDBY_OPCODE;
  xmitBuffer[1] = LORA_STDBY_XOSC;
  if(HAL_SPI_Transmit(hspi, xmitBuffer, 2, 100) != HAL_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the packet type to LoRa */
  xmitBuffer[0] = LORA_SET_PACKET_TYPE_OPCODE;
  xmitBuffer[1] = LORA_PACKET_TYPE_LORA;
  if(HAL_SPI_Transmit(hspi, xmitBuffer, 2, 100) != HAL_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the frequency to 915 MHz  */
  /* See datasheet for calculation */
  xmitBuffer[0] = LORA_SET_RF_FREQUENCY_OPCODE;
  xmitBuffer[1] = 0x34;
  xmitBuffer[2] = 0x03;
  xmitBuffer[3] = 0x00;
  xmitBuffer[4] = 0xA0;
  if(HAL_SPI_Transmit(hspi, xmitBuffer, 5, 100) != HAL_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the power amplifier configuration */
  /* For now set output power to +14 dBm   */
  /* See datasheet page 74                 */
  xmitBuffer[0] = LORA_SET_PA_CONFIG_OPCODE;
  xmitBuffer[1] = 0x02; /* paDutyCycle */
  xmitBuffer[2] = 0x02; /* hpMax */
  xmitBuffer[3] = 0x00; /* devSel */
  xmitBuffer[4] = 0x01; /* paLut */
  if(HAL_SPI_Transmit(hspi, xmitBuffer, 5, 100) != HAL_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the TX parameters */
  xmitBuffer[0] = LORA_SET_TX_PARAMS_OPCODE;
  xmitBuffer[1] = 0x16; /* power */
  xmitBuffer[2] = 0x04; /* RampTime */
  if(HAL_SPI_Transmit(hspi, xmitBuffer, 3, 100) != HAL_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set DIO2 as the control for the RF switch */
  xmitBuffer[0] = LORA_SET_DIO2_AS_RF_SWITCH_CTRL;
  xmitBuffer[1] = 1; /* enable */
  if(HAL_SPI_Transmit(hspi, xmitBuffer, 2, 100) != HAL_OK)
  {
	  return(LoRa_ERROR);
  }


  return(LoRa_OK);

}

/*
 * LoRaGetStatus
 * Get the current status of the SX1262 chip
 * Arguments
 *  None
 * Return Value
 *  Status byte or 0xFF for error
 *  Status byte defined in SX1262 datasheet
 */
uint8_t LoRaGetStatus(void)
{
	/* Return the status byte from the SX1262 chip */
	uint8_t status = 0;
	uint8_t opcode = LORA_GET_STATUS_OPCODE;

	/* Can use simultaneous TransmitReceive because the SX1262 always sends status while master transmits a command */
	if(HAL_SPI_TransmitReceive(hspi, &opcode, &status, 1, 100) != HAL_OK)
	{
		return(0xFF);
	}

	return(status);
}

/*
 * LoRaSetTxContinuousWave
 * Set the chip into CW transmit mode for testing
 * Arguments
 *  None
 * Return Value
 *  LoRa_OK for success
 *  LoRa_ERROR in case of error
 */
LoRa_Status LoRaSetTxContinuousWave(void)
{
	uint8_t opcode = LORA_SET_TX_CONTINUOUS_WAVE;
	if(HAL_SPI_Transmit(hspi, &opcode, 1, 100) != HAL_OK)
	{
		return(LoRa_ERROR);
	}

	return(LoRa_OK);
}

/*
 * LoRaSetStandbyMode
 * Set the chip into Standby mode
 * Arguments
 *  None
 * Return Value
 *  LoRa_OK for success
 *  LoRa_ERROR in case of error
 */
LoRa_Status LoRaSetStandbyMode(void)
{
	uint8_t buffer[2];
	buffer[0] = LORA_SET_STANDBY_OPCODE;
	buffer[1] = LORA_STDBY_XOSC;
	if(HAL_SPI_Transmit(hspi, buffer, 2, 100) != HAL_OK)
	{
		return(LoRa_ERROR);
	}

	return(LoRa_OK);
}
