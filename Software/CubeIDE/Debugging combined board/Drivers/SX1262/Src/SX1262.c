/*
 * SX1262 LoRa transceiver driver
 * Copyright 2023 NSE Labs, LLC
 * Free to use
 *
 * Adapted for use with the STM32WLE series of processors with the SX1262 on board
 *
 *
 */
#include <stdbool.h>
#include <SX1262.h>
#include "main.h"

/* function definitions from HAL library */
void              SUBGHZSPI_Init(uint32_t BaudratePrescaler);
void              SUBGHZSPI_DeInit(void);
HAL_StatusTypeDef SUBGHZSPI_Transmit(SUBGHZ_HandleTypeDef *hsubghz, uint8_t Data);
HAL_StatusTypeDef SUBGHZSPI_Receive(SUBGHZ_HandleTypeDef *hsubghz, uint8_t *pData);
HAL_StatusTypeDef SUBGHZ_WaitOnBusy(SUBGHZ_HandleTypeDef *hsubghz);
HAL_StatusTypeDef SUBGHZ_CheckDeviceReady(SUBGHZ_HandleTypeDef *hsubghz);

SUBGHZ_HandleTypeDef *hspi;

/*
 * WaitWhileBusy
 * wait for the busy line on the SX1262 to go low indicating not busy
 * Arguments
 *  timeout in milliseconds (maximum time to wait for line to go low)
 * Return Value
 *  LoRa_OK for success
 *  LoRa_TIMEOUT for timeout
 */
static LoRa_Status WaitWhileBusy(void)
{
	HAL_StatusTypeDef retval;

	retval = SUBGHZ_WaitOnBusy(hspi);
	if(retval == HAL_OK)
	{
		return(LoRa_OK);
	}
	else if (retval == HAL_TIMEOUT)
	{
		return(LoRa_TIMEOUT);
	}
	else
	{
		return(LoRa_ERROR);
	}
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
	int x;

	/* make sure chip isn't busy */
	if(WaitWhileBusy() != LoRa_OK)
	{
		return(LoRa_ERROR);
	}
	for(x=0; x < numBytes; x++)
	{
		SUBGHZSPI_Transmit(hspi, buffer[x]);
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
	if(WaitWhileBusy() != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	SUBGHZSPI_Receive(hspi, buffer);

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
LoRa_Status LoRa_Init(SUBGHZ_HandleTypeDef *hspiPtr)
{
	uint8_t xmitBuffer[10];

  /* store the hspi pointer for future use */
  hspi = hspiPtr;


  /* Set NSS active */
  LL_PWR_SelectSUBGHZSPI_NSS();

  /* Reset SX1262 chip */
  LL_RCC_RF_EnableReset();
  HAL_Delay(1); 															/* delay 1mS */
  LL_RCC_RF_DisableReset();


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

  /* Set LoRa modulation parameters */
  xmitBuffer[0] = LORA_SET_MODULATION_PARAMS_OPCODE;
  xmitBuffer[1] = 7; 		/* Spreading factor */
  xmitBuffer[2] = 0x04; 	/* BW = 125 KHz */
  xmitBuffer[3] = 0x01;		/* Coding Rate = 4/5 */
  xmitBuffer[4] = 0;		/* Low data rate optimization off */
  if(SPI_Send(xmitBuffer, 5, false) != LoRa_OK)
  {
	  return(LoRa_ERROR);
  }

  /* Set DIO IRQ parameters */
  xmitBuffer[0] = LORA_SET_DIO_IRQ_PARAMS_OPCODE;
  xmitBuffer[1] = 0x02;	/* Enable timeout interrupt */
  xmitBuffer[2] = 0x02;	/* Enable RX done interrupt */
  xmitBuffer[3] = 0x02;	/* Map timeout to DIO1 */
  xmitBuffer[4] = 0x02; /* Map RX done to DIO1 */
  xmitBuffer[5] = 0;
  xmitBuffer[6] = 0;	/* No interrupts to DIO2 */
  xmitBuffer[7]	= 0;
  xmitBuffer[8]	= 0;	/* No interrupts to DIO3 */
  if(SPI_Send(xmitBuffer, 9, false) != LoRa_OK)
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


/*
 * LoRaTransmit
 * Send a message over the airwaves
 * Arguments
 *  pointer to message to send
 *  number of bytes to send
 * Return Value
 *  LoRa_OK for success
 *  LoRa_ERROR in case of error
 */
LoRa_Status LoRaTransmit(uint8_t *msg, uint8_t numBytes)
{
	uint8_t xmitBuffer[128];
	uint8_t i;

	if(numBytes > 127) return(LoRa_ERROR);

	/* Set base address of transmit and receive buffer */
	xmitBuffer[0] = LORA_SET_BUFFER_BASE_ADDRESS_OPCODE;
	xmitBuffer[1] = 0; /* Transmit buffer start address */
	xmitBuffer[2] = 0; /* Receive buffer start address */
	if(SPI_Send(xmitBuffer, 3, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Write the message to the transmit buffer on the SX1262 chip*/
	xmitBuffer[0] = LORA_WRITE_BUFFER_OPCODE;
	xmitBuffer[1] = 0; 		/* Offset of transmit buffer */
	for(i=2; i <= (numBytes+1); i++)
	{
		xmitBuffer[i] = *msg++;
	}
	if(SPI_Send(xmitBuffer, numBytes+2, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Set LoRa packet parameters */
	xmitBuffer[0] = LORA_SET_PACKET_PARAMS_OPCODE;
	xmitBuffer[1] = 0;
	xmitBuffer[2] = 0x08;		/* Preamble length (2 bytes) */
	xmitBuffer[3] = 0;			/* Variable length packet */
	xmitBuffer[4] = numBytes; 	/* Size of message */
	xmitBuffer[5] = 0;			/* CRC off */
	xmitBuffer[6] = 0;			/* Standard IQ setup */
	if(SPI_Send(xmitBuffer, 7, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Put the chip in transmit mode to send the message */
	xmitBuffer[0] = LORA_SET_TX_OPCODE;
	xmitBuffer[1] = 0;
	xmitBuffer[2] = 0;
	xmitBuffer[3] = 0; 	/* timeout value (3 bytes) set to zero for no timeout */
	if(SPI_Send(xmitBuffer, 4, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	if(WaitWhileBusy() != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	return(LoRa_OK);
}

/*
 * LoRaReceive
 * Go into receive mode and wait for a message
 * Arguments
 *  pointer to buffer to receive message
 *  timeout as specified in table 13-9 pg 72 of datasheet
 * Return Value
 *  LoRa_OK for success
 *  LoRa_ERROR in case of error
 */
LoRa_Status LoRaReceive(uint8_t *msg, uint32_t timeout)
{
	uint8_t SPIbuffer[10];
	uint8_t RxPayloadLength, RxStartBufferPointer;

	/* Set base address of transmit and receive buffer */
	SPIbuffer[0] = LORA_SET_BUFFER_BASE_ADDRESS_OPCODE;
	SPIbuffer[1] = 0; /* Transmit buffer start address */
	SPIbuffer[2] = 0; /* Receive buffer start address */
	if(SPI_Send(SPIbuffer, 3, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Set LoRa packet parameters */
	SPIbuffer[0] = LORA_SET_PACKET_PARAMS_OPCODE;
	SPIbuffer[1] = 0;
	SPIbuffer[2] = 0x08;		/* Preamble length (2 bytes) */
	SPIbuffer[3] = 0;			/* Variable length packet */
	SPIbuffer[4] = 128; 		/* Max size of message */
	SPIbuffer[5] = 0;			/* CRC off */
	SPIbuffer[6] = 0;			/* Standard IQ setup */
	if(SPI_Send(SPIbuffer, 7, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Put the chip in receive mode */
	SPIbuffer[0] = LORA_SET_RX_OPCODE;
	SPIbuffer[1] = (timeout & 0xFF0000) >> 16;
	SPIbuffer[2] = (timeout & 0xFF00) >> 8;
	SPIbuffer[3] = timeout & 0xFF;
	if(SPI_Send(SPIbuffer, 4, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	if(WaitWhileBusy() != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Clear IRQ status */
	SPIbuffer[0] = LORA_CLEAR_IRQ_STATUS_OPCODE;
	SPIbuffer[1] = 0x02; /* Clear timeout interrupt */
	SPIbuffer[2] = 0x02; /* Clear RX done interrupt */
	if(SPI_Send(SPIbuffer, 3, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}

	/* Get the receive buffer status */
	SPIbuffer[0] = LORA_GET_RX_BUFFER_STATUS_OPCODE;
	if(SPI_Send(SPIbuffer, 1, true) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}
	if(SPI_Receive(SPIbuffer, 3, false) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}
//	RxStatus = SPIbuffer[0];
	RxPayloadLength = SPIbuffer[1];
	RxStartBufferPointer = SPIbuffer[2];

	/* Read the packet data */
	SPIbuffer[0] = LORA_READ_BUFFER_OPCODE;
	SPIbuffer[1] = RxStartBufferPointer;
	if(SPI_Send(SPIbuffer, 2, true) != LoRa_OK)
	{
		return(LoRa_ERROR);
	}
	if(SPI_Receive(SPIbuffer, 1, true) != LoRa_OK) /* get the status byte */
	{
		return(LoRa_ERROR);
	}
	if(SPI_Receive(msg, RxPayloadLength, false) != LoRa_OK) /* get the packet message */
	{
		return(LoRa_ERROR);
	}

	return(LoRa_OK);
}

