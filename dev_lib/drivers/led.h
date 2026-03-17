#ifndef LED_H
#define LED_H

#include "stm32f4xx_hal.h"

typedef struct led_property {
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState on_level;       // 点亮状态时 led 引脚电平
} led_property_t;

typedef struct led_ctx {
    uint8_t is_init;
} led_ctx_t;

typedef struct led led_t;
struct led {
    const led_property_t property;
    led_ctx_t ctx;

    void (*on)(led_t *self);
    void (*off)(led_t *self);
    void (*toggle)(led_t *self);
};

void led_init(led_t *self);

#endif
