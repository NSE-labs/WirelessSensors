# CPU Board Rev A.1 Debugging

The two prototype boards were received in good order.

![Prototype Boards](Images/First%20round%20boards.JPG)


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

[Back to project outline](../README.md)