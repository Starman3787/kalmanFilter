#ifndef __DRIVERS_GTU7_H__
#define __DRIVERS_GTU7_H__

#include <stdint.h>

#define __DRIVERS_GTU7_PIN_RX__     (11)
#define __DRIVERS_GTU7_PIN_TX__     (10)

#define __DRIVERS_GTU7_BAUD_RATE__  (9600)

int8_t Gtu7_Init(void);

#endif