#include <Arduino.h>

const unsigned char PIN_SIGNAL = LED_BUILTIN;

void setup()
{
  pinMode(PIN_SIGNAL, OUTPUT);
}

void loop()
{
  digitalWrite(PIN_SIGNAL, digitalRead(PIN_SIGNAL) ^ 1);
  delay(500);
}
