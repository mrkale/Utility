/**
 * @name queuePushAverage
 *
 * @brief Push an element into a queue and calculate the average of the
 * elements.
 *
 * @copyright This program is free software; you can redistribute it and/or
 * modify it under the terms of the license GNU GPL v3
 * http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
 * License (MIT) for added code.
 *
 * @author Libor Gabaj
 */
#include <Arduino.h>

// Constants for queue
const byte QUEUE_SIZE = 5;

// Variables for queue
byte head_ = 0;
byte tail_ = 0;
byte count_ = 0;
uint32_t queue[QUEUE_SIZE];

// Push to queue with overwrite when full
void queuePush(unsigned long value)
{
  if (count_ == QUEUE_SIZE)
  {
    // Overwrite oldest
    head_ = (head_ + 1) % QUEUE_SIZE;
    count_--;
  }
  queue[tail_] = value;
  tail_ = (tail_ + 1) % QUEUE_SIZE;
  count_++;
}

// Compute average of all items in the queue
unsigned long queueAverage()
{
  if (count_ == 0)
  {
    return 0;
  }

  unsigned long sum = 0;
  byte idx = head_;

  for (byte i = 0; i < count_; i++)
  {
    sum += queue[idx];
    idx = (idx + 1) % QUEUE_SIZE;
  }

  return sum / count_;
}