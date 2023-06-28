# Wireless Sensor Project - Prototype Debugging

The two prototype boards were received in good order.

![Prototype Boards](Images/First%20round%20boards.JPG)

## CPU board

1. Plugged a micro USB cable into J1 on the board. The other end of the USB cable was connected to a standard wall charger supplying 5VDC. The green LED (D2) was observed to light. The output of U2 (3.3V regulator) was measured as 3.29 volts. All components of the power supply remained cool to the touch. Since the Power jumper was not installed, no voltage was supplied to the CPU. The micro USB cable was then disconnected.
2. Pin headers were soldered onto the top of the board for the Power jumper, the Run/Load jumper, and the 5-pin Debug port. Jumper blocks were installed in the Power jumper and in Run position of the Run/Load jumper.
3. A debug cable was fabricated to connect an STLINK-V3MINIE in-circuit debugger to the debug port. Pins were soldered to the edge connector on the STLINK-V3MINIE to support this connection.

![Debug connector](Images/Debug%20connector.JPG)

![Debug cable](Images/Debug%20cable.JPG)

4. The STLINK-V3MINIE USB was connected to the development PC. STM32 CubeIDE (v1.12.0) was started on the PC. The micro USB cable was reconnected to J1 to supply power to the board.
5. An attempt was made to load driver code to the board using the CubeIDE interface. CubeIDE reported that the processor was being held in reset so code could not be loaded.
6. Brief tracing of signals revealed that the RESET button was connected in a way that held the reset line low, thus keeping the processor in reset. The offending pin was gently desoldered and lifted from the board.
7. CubeIDE was now able to download and execute code, read processor registers, and step through code. 
8. Code was written to blink the blue LED (D1) and this executed successfully.
9. Code was written to set up the USB port as a virtual COM port and data was successfully printed on a terminal window on the PC.
10. Pin headers were soldered on the bottom of the board in the two 16-pin connectors on the edges of the board, to allow installation in a solderless breadboard.

If we choose to make another revision of this board, the following changes should be considered.

1. The reset button should be corrected. I was able to remove it from the board and rotate it 90 degrees so it now functions. But since the pins are not symmetrical it doesn't fit properly on the pads. For the prototype I was able to bridge the difference with solder.
2. The debug port could be changed to the standard 14-pin debug connector that comes standard on the STLINK-V3MINIE. This would remove the need for making a custom debug cable, and would also include a reset signal to allow for hardware reset of the processor from the debugger.
3. The board is too wide to fit on a standard solderless breadboard. It can be used on a large breadboard if it straddles two lanes. If the two 16-pin headers were 0.3 inches closer together it would fit in a single lane on a standard breadboard.
4. If possible, individual pin labels on the two 16-pin headers would be handy.

![Pin labels](Images/CPU%20Board%20labels.JPG)

Overall this is already a very useful and functional board.

## LoRa board

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

[NEXT: Gateway development](./LoRa%20gateway%20development.md)

[Back to project outline](../README.md)