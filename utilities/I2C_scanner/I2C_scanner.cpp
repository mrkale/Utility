/*
  NAME:
  I2C_scanner - Detect active I2C addresses for 7-bit addressing

  DESCRIPTION:
  The sketch initates transmition at each of 7 bit addresses. If some
  device answers at particular address, it is considered as present
  and active.
  - The sketch lists found addresses in hexadecimal and decimal format.
  - Results are displayed in serial monitor at 9600 baud.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj

  CREDITS:
  Nick Gammon, 20th April 2011 - Inspiration taken for thish sketch.
*/
#include <Arduino.h>
#include <Wire.h>
#include "gbj_serial_debug.h"

#define SKETCH "I2C_SCANNER 1.2.0"

// Range of scanned addresses
const byte ADDRESS_MIN = 0x00;
const byte ADDRESS_MAX = 0x7F;


void setup()
{
  SERIAL_BEGIN(9600)
  SERIAL_TITLE(SKETCH)
  SERIAL_TITLE("Libraries:")
  SERIAL_TITLE(GBJ_SERIAL_DEBUG_VERSION)
  SERIAL_DELIM

  // Print header
  SERIAL_TITLE("I2C scanner found address(es) within the range")

  // Buffer for formatted text
  char text[50];

  sprintf(text, "0x%02X - 0x%02X (%u - %u):",
    ADDRESS_MIN, ADDRESS_MAX,
    ADDRESS_MIN, ADDRESS_MAX
  );
  SERIAL_LOG1(text)

  // Active devices counter
  byte count = 0;

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
  // Scanning results
  if (count == 0)
  {
    SERIAL_TITLE("N/A")
  }
  sprintf(text, "*** Found %u device(s) ***", count);
  SERIAL_LOG1(text)
}

void loop() {}
