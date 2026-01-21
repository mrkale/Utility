/**
 * @name calculateHashCode
 *
 * @brief Calculate a hash code for a block of memory or an byte array.
 *
 * @copyright This program is free software; you can redistribute it and/or
 * modify it under the terms of the license GNU GPL v3
 * http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
 * License (MIT) for added code.
 *
 * @author Libor Gabaj
 */
#include <Arduino.h>

uint16_t calculateHashCode(const uint8_t *addr, uint16_t len)
{
  uint16_t hash = 0;
  uint16_t pow = 1;
  while (len--)
  {
    pow *= 31;
    hash += pow * *addr++;
  }
  return hash;
}