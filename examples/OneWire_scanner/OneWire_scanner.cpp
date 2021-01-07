/*
  NAME:
  OneWire_scanner - Detect active devices on the one-wire bus

  DESCRIPTION:
  The sketch searches active (communicating) devices on one-wire bus.
  - The sketch lists hardware ROM addresses of discovered devices.
  - The sketch check CRC of ROMs and lists wrong addresses.
  - Results are displayed in serial monitor.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "ONEWIRE_SCANNER 1.0.0"
#include <OneWire.h>

const unsigned char PIN_ONEWIRE = 4; // Connect your one-wire bus to this pin
const unsigned char ADDRESS_LEN = 8;
typedef uint8_t Address[ADDRESS_LEN];

OneWire ds(PIN_ONEWIRE);
Address address;

String textAddress(Address address)
{
  String text = "";
  char data[3];
  for (byte i = 0; i < ADDRESS_LEN; i++)
  {
    if (i)
      text += "-";
    sprintf(data, "%02X", address[i]);
    text += data;
  }
  return text;
}

void setup()
{
  Serial.begin(9600);
  Serial.println(); // Some sernum monitors display garbage at the begining
  Serial.println(SKETCH);
  Serial.println("---");
  byte deviceNum = 0;
  while (ds.search(address))
  {
    if (ds.crc8(address, 7) != address[7])
    {
      Serial.println();
      Serial.println("Wrong CRC for address" + textAddress(address));
      continue;
    }
    Serial.println(String(++deviceNum) + ". ROM: " + textAddress(address));
  }
  ds.reset_search();
  Serial.println("---");
  Serial.println("Devices: " + String(deviceNum));
}

void loop() {}
