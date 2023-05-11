/*
  NAME:
  I2C_scanner - Detect active I2C addresses for 7-bit addressing

  DESCRIPTION:
  The sketch initates transmition at each of 7 bit addresses. If some
  device answers at particular address, it is considered as present
  and active.
  - The sketch lists found addresses in hexadecimal and decimal format.
  - Results are displayed in serial monitor.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj

  CREDITS:
  Nick Gammon, 20th April 2011 - Inspiration taken for thish sketch.
*/
#include "gbj_serial_debug.h"
#include <Arduino.h>
#include <Wire.h>

#undef SERIAL_PREFIX
#define SERIAL_PREFIX "I2C"

// Range of scanned addresses
const byte ADDRESS_MIN = 0x00;
const byte ADDRESS_MAX = 0x7F;

// Buffer for formatted text
char text[25];
// Active devices counter
byte count = 0;

void setup()
{
  SERIAL_BEGIN(SERIAL_DEBUG_BAUD)
  SERIAL_DELIM

  sprintf(text,
          "0x%02X ~ 0x%02X (%u ~ %u)",
          ADDRESS_MIN,
          ADDRESS_MAX,
          ADDRESS_MIN,
          ADDRESS_MAX);
  SERIAL_VALUE("Scanner address range", text)

  // Address scanning
  Wire.begin();
  for (byte i = ADDRESS_MIN; i <= ADDRESS_MAX; i++)
  {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0)
    {
      sprintf(text, "%1u. 0x%02X (%u)", ++count, i, i);
      SERIAL_LOG1(text)
    }
  }
  SERIAL_VALUE("Found devices", count)
  SERIAL_DELIM
}

void loop() {}
