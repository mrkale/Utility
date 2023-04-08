/*
  NAME:
  Shift bytes in EEPROM

  DESCRIPTION:
  Firmware shifts defined number of byte from starting position by defined
  bytes forward or backwards in EEPROM.
  - Values shifted outside EEPROM range are lost.

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
const unsigned int EEPROM_BYTES = 4;
const int EEPROM_POSITON_SHIFT = 3;

unsigned int eepromSizeMax;
//******************************************************************************
// Utilities
//******************************************************************************
unsigned int eepromPositionMin()
{
  if (EEPROM_POSITON_SHIFT >= 0)
  {
    return EEPROM_POSITON_START;
  }
  else
  {
    return EEPROM_POSITON_START - EEPROM_BYTES + EEPROM_POSITON_SHIFT + 1;
  }
}
unsigned int eepromPositionMax()
{
  if (EEPROM_POSITON_SHIFT >= 0)
  {
    return EEPROM_POSITON_START + EEPROM_BYTES + EEPROM_POSITON_SHIFT - 1;
  }
  else
  {
    return EEPROM_POSITON_START;
  }
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
  unsigned int start, stop;
  if (EEPROM_POSITON_SHIFT >= 0)
  {
    start = EEPROM_POSITON_START;
    stop = EEPROM_POSITON_START + EEPROM_BYTES - 1 + EEPROM_POSITON_SHIFT;
  }
  else
  {
    stop = EEPROM_POSITON_START + EEPROM_BYTES - 1;
    start = EEPROM_POSITON_START + EEPROM_POSITON_SHIFT;
  }
  for (unsigned int i = start; i <= stop; i++)
  {
    SERIAL_VALUE_INDEX(i, "eeprom", EEPROM.read(i));
  }
}
void eepromShift()
{
  if (EEPROM_POSITON_SHIFT >= 0)
  {
    for (int i = EEPROM_POSITON_START + EEPROM_BYTES - 1;
         i >= static_cast<int>(EEPROM_POSITON_START);
         i--)
    {
      int j = i + EEPROM_POSITON_SHIFT;
      if (j > static_cast<int>(eepromSizeMax))
      {
        continue;
      }
      EEPROM.write(j, EEPROM.read(i));
    }
  }
  else
  {
    for (unsigned int i = EEPROM_POSITON_START;
         i < EEPROM_POSITON_START + EEPROM_BYTES;
         i++)
    {
      int j = i + EEPROM_POSITON_SHIFT;
      if (j < 0)
      {
        continue;
      }
      EEPROM.write(j, EEPROM.read(i));
    }
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
  SERIAL_VALUE("EEPROM bytes", EEPROM_BYTES)
  SERIAL_VALUE("EEPROM shift", EEPROM_POSITON_SHIFT)
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

  // Shift EEPROM
  eepromShift();

  // Dump EEPROM after shift
  SERIAL_LINE
  SERIAL_TITLE("EEPROM-AFTER")
  eepromDump();
  SERIAL_DELIM;
}

void loop() {}
