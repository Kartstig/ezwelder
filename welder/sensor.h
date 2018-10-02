#ifndef SENSOR_H_
#define SENSOR_H_

#define ROUND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define READ_COUNT 5

int averageRead(const int pin);

#endif // SENSOR_H_

