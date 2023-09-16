#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "stm32yyxx_ll_adc.h"
#include <SPI.h>
#include <SD.h>

const int chipSelect = A4;

#define LL_ADC_RESOLUTION LL_ADC_RESOLUTION_12B
#define ADC_RANGE 4096

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declaration for the INA219 voltage and current sensor
Adafruit_INA219 batteryMonitor(0x40);
Adafruit_INA219 panelMonitor(0x41);


void setup(void) 
{
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while(1)
    {
      Serial.println("Screen allocation failed");
      delay(1000);
    }
  }

  display.clearDisplay();
  display.display();
  
  // Initialize the INA219s.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! batteryMonitor.begin()) {
    while(1)
    {
      Serial.println("Failed to find battery INA219 chip");
      delay(1000);
    }
  }
  if (! panelMonitor.begin()) {
    while(1)
    {
      Serial.println("Failed to find panel INA219 chip");
      delay(1000);
    }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  batteryMonitor.setCalibration_16V_400mA();
  panelMonitor.setCalibration_16V_400mA();

  analogReadResolution(12);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("SD card failed, or not present");
  }
  else
  {
    Serial.println("SD card initialized.");
  }
  pinMode(PA0, OUTPUT); /* Charging LED */
  pinMode(PA1, OUTPUT); /* Done LED */
  pinMode(PA10, INPUT_PULLUP); /* Charging input */
  pinMode(PA15, INPUT_PULLUP); /* Done input */
}

void loop(void) 
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  float panelVoltage = 0;
  float panelCurrent = 0;
  float panelPower = 0;
  float Vcc = 0;
  String dataString = "";
  int chargeStatus = LOW;
  int doneStatus = LOW;

  Vcc = ((float)__LL_ADC_CALC_VREFANALOG_VOLTAGE(analogRead(AVREF), LL_ADC_RESOLUTION))/1000;
  
  shuntvoltage = batteryMonitor.getShuntVoltage_mV();
  busvoltage = batteryMonitor.getBusVoltage_V();
  current_mA = batteryMonitor.getCurrent_mA();
  power_mW = batteryMonitor.getPower_mW();
  panelVoltage = panelMonitor.getBusVoltage_V();
  panelCurrent = panelMonitor.getCurrent_mA();
  panelPower = panelMonitor.getPower_mW();
  
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);
  display.println(F("Battery  Panel  Vcc")); 
  display.print(busvoltage); display.println(F("V"));
  display.print(current_mA); display.println(F("mA"));
  display.print(power_mW); display.println(F("mW"));
  display.setCursor(56,8); 
  display.print(panelVoltage); display.println(F("V"));
  display.setCursor(56,16);
  display.print(panelCurrent); display.println(F("mA"));
  display.setCursor(56,24);
  display.print(panelPower); display.println(F("mW"));
  display.setCursor(96, 8);
  display.print(Vcc); display.println(F("V"));
  display.display();
  
  Serial.print("Bus Voltage:    "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage:  "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Battery Voltage:"); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Battery Current:"); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Battery Power:  "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  dataString += String(busvoltage) + "," + String(current_mA) + "," + String(power_mW) + "," + String(panelVoltage) + "," + String(panelCurrent) + "," + String(panelPower) + "," + String(Vcc);
  
  
  // open the log file
  File dataFile = SD.open("log.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }

  // Light up LEDs to reflect charging status */
  if (digitalRead(PA10) == HIGH) /* Charging status */
  {
    chargeStatus = LOW;
  }
  else
  {
    chargeStatus = HIGH;
  }

  if (digitalRead(PA15) == HIGH) /* Done status */
  {
    doneStatus = LOW;
  }
  else
  {
    doneStatus = HIGH;
  }

  digitalWrite(PA0, chargeStatus);
  digitalWrite(PA1, doneStatus);
  
  delay(2000);
}
