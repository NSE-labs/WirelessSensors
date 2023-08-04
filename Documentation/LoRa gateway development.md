# LoRa Gateway Development

Part of the wireless sensor project includes development of a LoRa gateway for the sensor devices to send their data to, which can then forward the data via WiFi and MQTT.

![Gateway diagram](Images/Gateway%20diagram.png)

The gateway should be located indoors where there is mains power, so it does not need to be a low power device. This off-the-shelf board was chosen for the initial implementation of the gateway.

![Gateway board](Images/Heltec%20container.JPG)

This board contains an ESP32 processor with WiFi and Bluetooth, an SX1262 LoRa transceiver, and a small OLED display. It comes pre-loaded with firmware and can be programmed using the Arduino IDE. Here is the board when first powered up - it goes through a series of displays and eventually shows this screen.

![Inital power up](Images/Heltec%20board.JPG)

Software was written using the Arduino IDE and some examples from Heltec to receive packets and display them on the OLED screen. This was used to successfully test the transmit capability of our custom LoRa board. Additonal software verified the transmit capability of the Heltec board and receive capability of the LoRa board.

![Transmit test](Images/Gateway%20receiving.JPG)

[Back to project outline](../README.md)