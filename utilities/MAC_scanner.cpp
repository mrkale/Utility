/*
  NAME:
  MAC_scanner - Detect MAC address of the ESP8266/ESP32

  DESCRIPTION:
  The sketch detect Media Access Control address of the microcontroller.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#else
  #error !!! Only ESP8266/ESP32 platforms are supported !!!
#endif
#include "gbj_serial_debug.h"

#undef SERIAL_PREFIX
#define SERIAL_PREFIX "MAC_scanner"

void setup()
{
  SERIAL_BEGIN(SERIAL_DEBUG_BAUD)
#ifndef SERIAL_NODEBUG
  delay(3000); // Wait for serial monitor consolidation
#endif
  SERIAL_VALUE("Project", PROJECT)
  SERIAL_VALUE("Version", MAIN_VERSION)
  SERIAL_ACTION("Platform...")
#if defined(ESP8266)
  SERIAL_ACTION_END("ESP8266")
#elif defined(ESP32)
  SERIAL_ACTION_END("ESP32")
#endif
  SERIAL_VALUE("MAC Address", WiFi.macAddress());
}

void loop() {}