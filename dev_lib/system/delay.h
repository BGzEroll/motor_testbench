#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx_hal.h"

void delay_us(uint32_t us);
uint32_t delay_get_us_tick(void);

#endif
