/*
  NAME:
  Scanning 1-wire bus and detecting all DS18 temperature sensors on it

  DESCRIPTION:
  The sketch detects all devices on the one wire bus.
  - Printed features:
    MODEL: Family name of sensors
    RESOLUTION: Temperature sensing resolution in bits and centigrade
    POWER: Mode of power supply
    ID: Identifier as a CRC8 value from address (last address byte)
    ADDRESS: Address in hexadecimal format
  - Results are displayed in serial monitor at 9600 baud.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include <Arduino.h>
#include <OneWire.h>
#include "gbj_dallas18.h"
#include "gbj_serial_debug.h"

#define SKETCH "DS18B20_SCANNER 1.5.0"

#define TAB SERIAL_F("\t")

// Hardware configuration
const unsigned char PIN_DS18B20 = 3;   // Usually on Arduino
// const unsigned char PIN_DS18B20 = D1;  // Usually on ESP8266

// Processing
gbj_dallas18 ds = gbj_dallas18(PIN_DS18B20);
gbj_dallas18_address_text addressText;
gbj_dallas18_id_text idText;
unsigned char deviceCount;

void setup()
{
  SERIAL_BEGIN(9600)
  SERIAL_TITLE(SKETCH)
  SERIAL_TITLE("Libraries:")
  SERIAL_TITLE(GBJ_SERIAL_DEBUG_VERSION)
  SERIAL_TITLE(GBJ_DALLAS18_VERSION)
  SERIAL_DELIM

  // Devices scanning
  SERIAL_TITLE("#\tMODEL\tRESOLUTION\t\tPOWER\t\tID\tADDRESS")
  while (ds.selectNext())
  {
    // Print order number
    SERIAL_CHAIN2(++deviceCount, TAB)
    // Print family name
    switch(ds.getFamilyCode())
    {
      case GBJ_DALLAS18_MODEL_DS18S20:
        SERIAL_ACTION("DS18S20/DS1820")
        break;
      case GBJ_DALLAS18_MODEL_DS1822:
        SERIAL_ACTION("DS1822")
        break;
      case GBJ_DALLAS18_MODEL_DS18B20:
        SERIAL_ACTION("DS18B20")
        break;
      case GBJ_DALLAS18_MODEL_DS18B25:
        SERIAL_ACTION("DS18B25")
        break;
      default:
        SERIAL_ACTION("UNKNOWN")
    }
    // Print resolution in bits and centigrade
    SERIAL_CHAIN4(TAB, ds.getResolutionBits(), SERIAL_F(" bits"), TAB);
    Serial.print(ds.getResolutionTemp(), 4);
    SERIAL_CHAIN2(SERIAL_F(" 'C"), TAB)
    // Print power mode
    switch (ds.getPowerMode())
    {
      case GBJ_DALLAS18_POWER_EXTERNAL:
        SERIAL_ACTION("External")
        break;
      case GBJ_DALLAS18_POWER_PARASITE:
        SERIAL_ACTION("Parasite")
        break;
    }
    // Print identifier and address
    ds.printId(idText);
    ds.printAddress(addressText);
    SERIAL_LOG4(TAB, idText, TAB, addressText)
  }
  // Scanning results
  SERIAL_LINE
  if (deviceCount == 0)
  {
    SERIAL_TITLE("N/A")
    return;
  }
  SERIAL_LOG3(SERIAL_F("*** Found "), deviceCount, SERIAL_F(" device(s) ***"))
}

 void loop(){}
