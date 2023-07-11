# Wireless sensor range testing

## Setup

1. An enclosure design for the Heltec board was found on Thingiverse. 
[Link](https://www.thingiverse.com/thing:3783207)
2. The enclosure was printed and assembled. A LiPo battery was added that did not fit in the enclosure. While not ideal, this made the assembly sufficiently portable for range testing.
![Assembled enclosure](Images/Heltec%20temporary%20case.JPG)
3. A second empty printed enclosure was used to hold the antenna attached to the LoRa board in a vertical orientation.
![LoRa board antenna](Images/LoRa%20board%20antenna.JPG)
4. The prototype CPU/LoRa board combination was programmed to send a LoRa message every second. The message contained an incrementing counter. The communication parameters were:
- Frequency: 915 MHz
- Power: +22 dBm
- Spreading Factor: 7
- Bandwidth: 125 kHz
- Coding Rate: 4/5
- Low data rate optimization off
- Preamble Length: 2 bytes
- Variable length packet
- CRC off
5. The Heltec board was programmed to receive the message and display the message contents along with the measured RSSI and a signal strength bar graph based on RSSI.
6. The protoype CPU/LoRa board (transmitter) was placed roughly in the center of the lab building, first floor (ground level), on a table top that was 28 inches off the floor. 

Keep in mind that the lab building was built in 1870 and has 18-inch thick brick walls, so is not very RF friendly.

## Testing

Testing consisted of walking around the neighborhood and watching the signal strength and packet count on the Heltec board. It was determined that consistent packet reception could be reliably maintained out to a radius of approximately 1700 feet (0.32 miles, 0.52 km). This was roughly six blocks in the city where testing was done. Beyond that distance some packets were dropped.

## Conclusion

With further optimization of spreading factor, bandwidth, etc. it is likely this range could be extended even farther. But the range already vastly exceeds the requirements of this project. Test passed.


