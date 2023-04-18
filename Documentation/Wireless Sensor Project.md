
# Wireless Sensor Project

The purpose of this project is to develop hardware and software for small wireless sensors. The sensors could be used to monitor parameters such as temperature, humidity, light levels, or actuation of a switch or contact.

## Requirements

1. The sensors should be independent of mains power.
1. The sensors should be able to communicate with either a single-board computer (example: Raspberry Pi) or smartphone, or both.
1. Communication range for the sensors should cover an average house at minimum. Ideally it would cover cases where the sensor may be placed in a greenhouse, chicken coop, or mailbox on the same property.
1. The sensors should be reasonably rugged and protected from moisture, but are not necessarily intended for outdoor mounting or submersion in liquids.
1. The data rate should support a few parameters sent once per minute. High speed data such as audio or video are not required.
1. Size of the sensor could be as large as a college textbook, but smaller is better.
1. Accuracy will vary according to the sensors used. Provisions will need to be made in the packaging to avoid self-heating in the case of temperature sensing.
1. If replaceable batteries are used, battery life should be at least 3 months.
1. If rechargeable batteries are used, battery life should be at least 1 month.
1. It should be possible to run the sensors on solar power by connecting an external solar panel. 

## Design

### Communication

We are looking for a communication method that prioritizes long range and low power over bandwidth, since the bandwidth needs of these sensors are small and they need to run on battery power.

#### WiFi

WiFi has been used up to now for wireless sensors at NSE Labs, mostly using ESP8266 and ESP32 modules which are cheap and readily available. WiFi has several disadvantages however:
- Limited range, effectively only useful within the home.
- WiFi credentials need to be hard-coded into the sensor, or a mechanism such as a web interface needs to be used to configure the sensor for the local WiFi network.
- WiFi requires high power, and if the sensor goes to sleep it must reconnect to the WiFi network when it wakes up. Not suitable for battery power.

#### Bluetooth and BLE

The initial thought was to use Bluetooth Low Energy (BLE) to replace WiFi as the communication method. It solves a couple of problems with WiFi, namely the need for credentials and the high power requirements. However it uses the same frequency range as WiFi and supports higher data rates than needed, so the range ends up being comparable to WiFi. Blueooth version 5 introduced a "long range" mode which reduces the bandwidth to as low as 125 kbps, which is still much more bandwidth than needed by this application. The claim is "up to" 1000m of range outdoors, but with the 2.4 GHz frequency used, the range is highly dependent on what objects are around, and it doesn't travel through walls very well.

#### LoRa

LoRa is short for "Long Range" and appears to be an ideal communication mode to use for these sensors. It is low bandwidth, with an adjustable data rate from 0.3 to 50 kbps. The lower data rates provide the longest range. The maximum range will vary based on a lot of factors but is typically several kilometers at least, easily enough to cover most properties.

Looking at the SX1262 chip from Semtech as the LoRa transceiver. There is also a module with this chip along with a processor core called the ASR6501, however it does not seem to be readily available.
