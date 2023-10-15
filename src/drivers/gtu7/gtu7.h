#ifndef DRIVERS_GTU7_H__
#define DRIVERS_GTU7_H__

#include <stdint.h>

#define GTU7_PIN_RX     (11)
#define GTU7_PIN_TX     (10)

#define GTU7_BAUD_RATE  (9600)

int8_t Gtu7_Init(void);

#endif