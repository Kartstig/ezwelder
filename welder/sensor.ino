#include "sensor.h"

int averageRead(const int pin) {
  int i=0;
  int sum = 0;
  while (i < READ_COUNT ) {
    int read = analogRead(pin);
    sum += read;
    i++;
  }
  const float avg = (float) sum / (float) READ_COUNT;
  return ROUND(avg);
}

