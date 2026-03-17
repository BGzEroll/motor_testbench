#ifndef LED_DEV_H
#define LED_DEV_H

#include "led.h"
#include "main.h"

extern led_t onBoradBuleLed;
extern led_t onBoradGreenLed;

void led_dev_init(void);
void led_dev_proc(void);

#endif
