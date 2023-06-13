
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

We are looking for a communication method that prioritizes long range and low power over bandwidth, since the bandwidth needs of these sensors are small and they need to run on battery power. Here are some examples of communication methods currently used.

#### WiFi

WiFi has been used up to now for wireless sensors at NSE Labs, mostly using ESP8266 and ESP32 modules which are cheap and readily available. WiFi has several disadvantages however:
- Limited range, effectively only useful within the home.
- WiFi credentials need to be hard-coded into the sensor, or a mechanism such as a web interface needs to be used to configure the sensor for the local WiFi network.
- WiFi requires high power, and if the sensor goes to sleep it must reconnect to the WiFi network when it wakes up. Not suitable for battery power.

#### Bluetooth and BLE

The initial thought was to use Bluetooth Low Energy (BLE) to replace WiFi as the communication method. It solves a couple of problems with WiFi, namely the need for credentials and the high power requirements. However it uses the same frequency range as WiFi and supports higher data rates than needed, so the range ends up being comparable to WiFi. Blueooth version 5 introduced a "long range" mode which reduces the bandwidth to as low as 125 kbps, which is still much more bandwidth than needed by this application. The claim is "up to" 1000m of range outdoors, but with the 2.4 GHz frequency used, the range is highly dependent on what objects are around, and it doesn't travel through walls very well.

#### LoRa

LoRa is short for "Long Range" and appears to be an ideal communication mode to use for these sensors. It uses Chirp Spread Spectrum modulation which improves immunity to RF noise and resistance to multipath fading. It is low bandwidth, with an adjustable data rate from 0.3 to 50 kbps. The lower data rates provide the longest range. The maximum range will vary based on a lot of factors but is typically several kilometers at least, easily enough to cover most properties. One downside is that the lower frequencies used (902-928 MHz in the U.S.) requires a larger antenna than devices operating at 2.4 GHz.

#### 433 MHz

A number of consumer devices use this frequency (typically 433.92 MHz) to send low bandwidth data from battery powered devices. Examples include home weather stations, wireless doorbells, and garage door openers. They typically use simple Amplitude Shift Keying modulation and have a range of 90 meters or less. Communication is one-way from the transmitter to the receiver.

#### Zigbee

Zigbee is based on IEEE 802.15.4 which is a standard for Wireless Personal Area Networks (WPANs). It is intended for low power, low data rate, and close proximity (short range). Range is listed as 10-100 meters. Zigbee does support a mesh network, so devices can retransmit to other devices to extend the range. It can operate in the 2.4 GHz band but also in the 915 MHz band. It has been used for home automation sensors, thermostats, switches, lights, outlets, door and window sensors, and door locks. Philips Hue lights use Zigbee, for example. Because of its mesh network design the communications will improve as more devices are added.

#### Z-Wave Plus

Z-Wave is another communication method intended for home automation devices. It operates in the 915 MHz band to avoid interference with WiFi. It is a mesh network like Zigbee. This is a proprietary standard managed by a group of companies called the Z-Wave Alliance, and requires a license from Silicon Laboratories to develop.

#### Thread and Matter

Matter is a new home automation standard intended to allow interoperability between a wide range of home automation devices. It addresses the proliferation of proprietary hubs needed to connect devices today. It is intended to assure consumers that if they buy a device with the Matter emlem on the box, it will just work in their home automation system. It remains to be seen if Matter will become the one standard to rule them all, or just another addition to the list of home automation standards.

The Matter standard allows for devices that communicate via WiFi, Bluetooth, and a new protocol called Thread. Thread is based on Zigbee at the hardware level, but changes the software so that the device can appear directly on the internet with an IPv6 address. This involves quite a bit of protocol and security overhead as you would expect, but makes routing of messages between the Thread network and the Internet easy and straightforward. 

The Thread network is a mesh network with some devices designated as routers and some designated as end devices. Since it is based on Zigbee hardware the range is the same, 10-100 meters.

### Hardware Design

Based on the above list of communication methods, LoRa is selected as the choice for this design.

Two small prototype boards were designed and built for initial testing and debugging. One contains an ST Microelectronics STM32L151C8T6 microcontroller. This microcontroller was chosen due to its low power consumption, ready availability of development and debugging tools, and the fact that it was in stock at JLCPCB and listed as a "basic" part there, meaning no setup charge to load the part.

The second board has an SX1262 LoRa transceiver and associated RF circuitry including a u.FL connector for external antenna.

![CPU board](Images/CPU%20board.png)

![LoRa board](Images/LoRa%20board.png)

![Actual boards](Images/First%20round%20boards.JPG)