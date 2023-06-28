/*
 * SX1262 LoRa transceiver driver
 * Copyright 2023 NSE Labs, LLC
 * Free to use
 *
 * The SPI port used to communicate with the SX1262 must be set up with NSS disabled.
 * NSS must be connected to a GPIO port labeled LoRa_NSS. The SPI controller in the STM32
 * will not toggle NSS as the SX1262 expects, so NSS is set/cleared via GPIO in this driver.
 *
 *
 */
#include <stdbool.h>
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

	uint32_t startTime;

	startTime = HAL_GetTick();

	while(HAL_GPIO_ReadPin(LoRa_BUSY_GPIO_Port, LoRa_BUSY_Pin) == GPIO_PIN_SET)
	{
		if((HAL_GetTick() - startTime) > timeout)
		{
			return(LoRa_TIMEOUT);
		}
	}

	return(LoRa_OK);
}

/*
 * SPI_Send
 * Send bytes to the SX1262 via SPI
 * Arguments
 *  Pointer to the byte(s) to send
 *  Number of bytes to send
 *  moreToCome
 *   if True then leave NSS low because more is coming in this sequence
 *   if False the bring NSS high at the end of the transmission
 * Return Value
 *  LoRa_OK for success
 *  LoRa_ERROR in case of error
 *
 */
static LoRa_Status SPI_Send(uint8_t* buffer, uint16_t numBytes, bool moreToCome)
{

	/* make sure chip isn't busy */
	if(WaitWhileBusy(10) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Set NSS low */
	HAL_GPIO_WritePin(LoRa_NSS_GPIO_Port, LoRa_NSS_Pin, GPIO_PIN_RESET);

	if(HAL_SPI_Transmit(hspi, buffer, numBytes, 100) != HAL_OK)
	{
		return(LoRa_ERROR);
	}

	if(moreToCome != true)
	{
		/* Set NSS high */
		HAL_GPIO_WritePin(LoRa_NSS_GPIO_Port, LoRa_NSS_Pin, GPIO_PIN_SET);
	}

	return(LoRa_OK);
}

/*
 * SPI_Receive
 * Get bytes from the SX1262 via SPI
 * Arguments
 *  Pointer to the receive buffer
 *  Number of bytes to receive
 *  moreToCome
 *   if True then leave NSS low because more is coming in this sequence
 *   if False the bring NSS high at the end of the transmission
 * Return Value
 *  LoRa_OK for success
 *  LoRa_ERROR in case of error
 *
 */
static LoRa_Status SPI_Receive(uint8_t* buffer, uint16_t numBytes, bool moreToCome)
{

	/* make sure chip isn't busy */
	if(WaitWhileBusy(10) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Set NSS low */
	HAL_GPIO_WritePin(LoRa_NSS_GPIO_Port, LoRa_NSS_Pin, GPIO_PIN_RESET);

	if(HAL_SPI_Receive(hspi, buffer, numBytes, 100) != HAL_OK)
	{
		return(LoRa_ERROR);
	}

	if(moreToCome != true)
	{
		/* Set NSS high */
		HAL_GPIO_WritePin(LoRa_NSS_GPIO_Port, LoRa_NSS_Pin, GPIO_PIN_SET);
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

  /* Set NSS high */
  HAL_GPIO_WritePin(LoRa_NSS_GPIO_Port, LoRa_NSS_Pin, GPIO_PIN_SET);

  /* Reset SX1262 chip */
  HAL_GPIO_WritePin(LoRa_NRST_GPIO_Port, LoRa_NRST_Pin, GPIO_PIN_RESET); 	/* Pull NRST low */
  HAL_Delay(1); 															/* delay 1mS */
  HAL_GPIO_WritePin(LoRa_NRST_GPIO_Port, LoRa_NRST_Pin, GPIO_PIN_SET);		/* Set NRST high */

  /* Set the chip to Standby mode and using the internal oscillator */
  xmitBuffer[0] = LORA_SET_STANDBY_OPCODE;
  xmitBuffer[1] = LORA_STDBY_RC;
  if(SPI_Send(xmitBuffer, 2, false) != LoRa_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the packet type to LoRa */
  xmitBuffer[0] = LORA_SET_PACKET_TYPE_OPCODE;
  xmitBuffer[1] = LORA_PACKET_TYPE_LORA;
  if(SPI_Send(xmitBuffer, 2, false) != LoRa_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the frequency to 915 MHz  */
  /* See datasheet for calculation */
  xmitBuffer[0] = LORA_SET_RF_FREQUENCY_OPCODE;
  xmitBuffer[1] = 0x39;
  xmitBuffer[2] = 0x30;
  xmitBuffer[3] = 0x00;
  xmitBuffer[4] = 0x00;
  if(SPI_Send(xmitBuffer, 5, false) != LoRa_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the power amplifier configuration */
  /* See datasheet page 74                 */
  xmitBuffer[0] = LORA_SET_PA_CONFIG_OPCODE;
  xmitBuffer[1] = 0x04; /* paDutyCycle */
  xmitBuffer[2] = 0x07; /* hpMax */
  xmitBuffer[3] = 0x00; /* devSel */
  xmitBuffer[4] = 0x01; /* paLut */
  if(SPI_Send(xmitBuffer, 5, false) != LoRa_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set the TX parameters */
  xmitBuffer[0] = LORA_SET_TX_PARAMS_OPCODE;
  xmitBuffer[1] = 22; /* power +22 dBm   */
  xmitBuffer[2] = LORA_STANDARD_RAMPTIME;
  if(SPI_Send(xmitBuffer, 3, false) != LoRa_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set DIO2 as the control for the RF switch */
  xmitBuffer[0] = LORA_SET_DIO2_AS_RF_SWITCH_CTRL_OPCODE;
  xmitBuffer[1] = 1; /* enable */
  if(SPI_Send(xmitBuffer, 2, false) != LoRa_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set regulator mode to use the DC-DC converter */
  xmitBuffer[0] = LORA_SET_REGULATOR_MODE_OPCODE;
  xmitBuffer[1] = 1; /* Use DC-DC converter */
  if(SPI_Send(xmitBuffer, 2, false) != LoRa_OK)
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

	if(SPI_Send(&opcode, 1, true) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	if(SPI_Receive(&status, 1, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	return(status);
}

/*
 * LoRaSetTxPower
 * Set the RF transmit power to specified level
 * Arguments
 *  Transmit power in dBm (-9 to +22 dBm)
 * Return Value
 *  LoRa_OK for success
 *  LoRa_ERROR in case of error
 */
LoRa_Status LoRaSetTxPower(int8_t power)
{
	int8_t buffer[3];

	if (power > 22) power = 22;
	if (power < -9) power = -9;

	buffer[0] = LORA_SET_TX_PARAMS_OPCODE;
	buffer[1] = power;
	buffer[2] = LORA_STANDARD_RAMPTIME;
	if(SPI_Send((uint8_t *)buffer, 3, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	return(LoRa_OK);
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
	uint8_t opcode = LORA_SET_TX_CONTINUOUS_WAVE_OPCODE;
	if(SPI_Send(&opcode, 1, false) != LoRa_OK)
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
	buffer[1] = LORA_STDBY_RC; /* Use internal RC oscillator to save power */
	if(SPI_Send(buffer, 2, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	return(LoRa_OK);
}