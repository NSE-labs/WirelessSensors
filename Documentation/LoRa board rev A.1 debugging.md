# LoRa Board Rev A.1 Debugging

1. A pin header was soldered to the 12-pin connector at the edge of the board.
2. The board was installed in a solderless breadboard, along with the CPU board.
3. A 915 MHz antenna was connected to J1 on the board.

![Initial breadboard](Images/Inital%20Breadboard.JPG)

4. A logic analyzer was connected to monitor all lines between the processor board and the LoRa board.

![Logic analyzer](Images/Breadboard%20with%20Logic%20Analyzer.JPG)

5. Code was written to initialize the SPI port, pull the NRST line high, send the SPI command to get the chip status, and receive the result. Below is the resulting successful communications as shown on the logic analyzer.

![Status communications](Images/SPI%20status%20communications.JPG)

6. The SX1262 device driver code was written to initialize the various chip registers and put the chip into a test mode where it transmits a carrier at 915 MHz. In the timing diagram below you can see the initialization sequence.
    1. NRST is pulled low to reset the chip.
    2. NRST goes high and BUSY stays high while the chip executes its reset sequence.
    3. Once BUSY goes low a bunch of configuration commands are sent, eventually resulting in DIO2 going high indicating the chip is transmitting.
    
![Startup sequence](Images/Initialization%20and%20transmit.JPG)

7. Next step was to connect the RF output of the LoRa board to the HP8568B Spectrum Analyzer. But prior to that, the spectrum analyzer was calibrated according to the manual. Below is the measurement screen after completing calibration, measuring the internal precision 20MHz -10dBm signal. The measurment reads exactly -10dBm at 20MHz.

![Cal result](Images/Spectrum%20analyzer%20cal%20result.JPG)

8. Now the RF output of the LoRa board was connected to the spectrum analyzer.

![Spectrum analyzer test setup](Images/Spectrum%20analyzer%20test%20setup.JPG)

Here's an example of an RF output measurement at maximum output settings. Note the frequency measured at 914.98674 MHz versus the target frequency of 915 MHz, a difference of 0.1%. It may be interesting to try adjusting the crystal capacitor loading registers to see what effect that might have.

![RF output measurement](Images/RF%20Power%20Measurement.JPG)

And here is the measured RF output based on power amplifier settings recommended in the data sheet.

![Measured RF output](Images/RF%20Output%20Power%20Graph.png)

Now we've reached the point where it's time to test actual LoRa communications. So we'll shift gears to develop a basic LoRa gateway for this board to talk to. 

[Gateway development](./LoRa%20gateway%20development.md)

9. Once the gateway was up and running, it was used to receive and display packets, thus verifying the transmit functionality of the LoRa board.

![Transmit test](Images/Gateway%20receiving.JPG)

10. Further testing verified the transmit capability of the gateway board, and receiving functionality of the LoRa board.

[Back to project outline](../README.md)