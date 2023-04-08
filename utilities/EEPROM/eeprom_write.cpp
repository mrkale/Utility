/*
  NAME:
  Write testing bytes to EEPROM

  DESCRIPTION:
  Firmware generates random values and writes them to the memory.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#if defined(__AVR__)
  #include <Arduino.h>
  #include <inttypes.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif
#include "gbj_serial_debug.h"
#include <EEPROM.h>

#undef SERIAL_PREFIX
#define SERIAL_PREFIX "main"
//******************************************************************************
// Constants and variables
//******************************************************************************
const unsigned int EEPROM_POSITON_START = 0;
const unsigned int EEPROM_POSITON_COUNT = 4;

std::vector<byte> values(EEPROM_POSITON_COUNT);
unsigned int eepromSizeMax;
//******************************************************************************
// Utilities
//******************************************************************************
void generateValues()
{
  randomSeed(analogRead(0) + millis());
  for (byte i = 0; i < values.size(); i++)
  {
    values[i] = random(1, 255);
  }
}
unsigned int eepromPositionMin()
{
  return EEPROM_POSITON_START;
}
unsigned int eepromPositionMax()
{
  return EEPROM_POSITON_START + values.size() - 1;
}
unsigned int eepromBegin()
{
#if defined(__AVR_ATmega328P__)
  return 1024;
#elif defined(PARTICLE)
  return 2047;
#elif defined(ESP8266) || defined(ESP32)
  EEPROM.begin(constrain(eepromPositionMax(), 4, 4096));
  return 4096;
#else
  return 0;
#endif
}
void eepromDump()
{
  for (unsigned int i = eepromPositionMin(); i <= eepromPositionMax(); i++)
  {
    SERIAL_VALUE_INDEX(i, "value", EEPROM.read(i));
  }
}
void eepromWrite()
{
  for (unsigned int i = 0; i < values.size(); i++)
  {
    unsigned int j = i + EEPROM_POSITON_START;
    if (j > eepromSizeMax)
    {
      break;
    }
    EEPROM.write(j, values[i]);
  }
#if defined(ESP8266) || defined(ESP32)
  EEPROM.commit();
#endif
}
//******************************************************************************
// Setup
//******************************************************************************
void setup()
{
  SERIAL_BEGIN(SERIAL_DEBUG_BAUD);
#ifndef SERIAL_NODEBUG
  delay(2000); // Wait for serial monitor consolidation
#endif
  SERIAL_VALUE("Version", MAIN_VERSION);
  SERIAL_ACTION("Platform...");
#if defined(__AVR__)
  SERIAL_ACTION_END("Arduino");
#elif defined(ESP8266)
  SERIAL_ACTION_END("ESP8266");
#elif defined(ESP32)
  SERIAL_ACTION_END("ESP32");
#elif defined(PARTICLE)
  SERIAL_ACTION_END("PARTICLE");
#endif
  SERIAL_DELIM;
  SERIAL_VALUE("EEPROM start", EEPROM_POSITON_START)
  SERIAL_VALUE("EEPROM values", values.size())
  // Init EEPROM
  eepromSizeMax = eepromBegin();
  if (eepromPositionMax() > eepromSizeMax)
  {
    SERIAL_VALUE("New maximal EEPROM position outside valid range",
                 eepromSizeMax);
    return;
  }
  // Dump EEPROM before shift
  SERIAL_LINE
  SERIAL_TITLE("EEPROM-BEFORE")
  eepromDump();

  // Write to EEPROM
  generateValues();
  eepromWrite();

  // Dump EEPROM after shift
  SERIAL_LINE
  SERIAL_TITLE("EEPROM-AFTER")
  eepromDump();
  SERIAL_DELIM;
}

void loop() {}
