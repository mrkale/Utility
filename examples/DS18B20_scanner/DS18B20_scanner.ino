/*
  NAME:
  Scanning one-wire bus and detecting all DS18B20 temperature sensors on it.

  DESCRIPTION:
  The sketch detects all sensors of particular family on the one-wire bus.
  - Printed bus features:
    - Power mode (parasitic, external)
    - Total number of active devices on the bus (not only sensors)
    - Total number of active sensors on the bus (DS18B20)
  - Printed sensor features:
    - Temperature sensing resolution in bits and centigrades
    - Identifier as a CRC8 value from address (last address byte)
    - Address in hexadecimal format
    - Alarm condition setting
  - Results are displayed in serial monitor at 9600 baud.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "DS18B20_SCANNER 1.6.0"
#include "gbj_ds18b20.h"

// Hardware configuration
const unsigned char PIN_ONEWIRE = 4; // Usually on Arduino
// const unsigned char PIN_ONEWIRE = D1;  // Usually on ESP8266

gbj_ds18b20 ds = gbj_ds18b20(PIN_ONEWIRE);
char buffer[50];

String textAddress(gbj_ds18b20::Address address)
{
  String text = "";
  char data[3];
  for (byte i = 0; i < gbj_ds18b20::ADDRESS_LEN; i++)
  {
    if (i)
      text += "-";
    sprintf(data, "%02X", address[i]);
    text += data;
  }
  return text;
}

void errorHandler()
{
  switch (ds.getLastResult())
  {
  case gbj_ds18b20::SUCCESS:
    Serial.println("SUCCESS");
    break;

  case gbj_ds18b20::END_OF_LIST:
    Serial.println("END_OF_LIST");
    break;

  case gbj_ds18b20::ERROR_NO_DEVICE:
    Serial.println("ERROR_NO_DEVICE");
    break;

  case gbj_ds18b20::ERROR_CRC_ADDRESS:
    Serial.println("ERROR_CRC_ADDRESS");
    break;

  case gbj_ds18b20::ERROR_CRC_SCRATCHPAD:
    Serial.println("ERROR_CRC_SCRATCHPAD");
    break;

  default:
    Serial.println("Unknown error");
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println(); // Some sernum monitors display garbage at the begining
  Serial.println(SKETCH);
  Serial.println("---");
  Serial.println("ONE-WIRE FEATURES");
  Serial.println();
  Serial.println("Powered: " +
                 String(ds.isPowerParasite() ? "Parasite" : "External"));
  Serial.println("Devices: " + String(ds.getDevices()));
  Serial.println("Sensors: " + String(ds.getSensors()));
  Serial.println("---");
  Serial.println("DS18B20 SENSORS");
  Serial.println();
  uint8_t deviceNum = 0;
  while (ds.isSuccess(ds.sensors()))
  {
    Serial.println(String(++deviceNum) + ". Id: " + String(ds.getId()));
    Serial.println("Address: " + String(textAddress(ds.getAddressRef())));
    Serial.println("Resolution: " + String(ds.getResolutionBits()) + " bits" + ", " +
                   String(ds.getResolutionTemp(), 4) + " 'C");
    Serial.println("Alarm Low: " + String(ds.getAlarmLow()) + " 'C");
    Serial.println("Alarm High: " + String(ds.getAlarmHigh()) + " 'C");
    Serial.println("---");
  }
  errorHandler();
}

void loop() {}
