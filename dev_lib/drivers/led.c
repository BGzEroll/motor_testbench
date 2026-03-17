#include "led.h"

/**
 * @brief 点亮 led
 */
static void on(led_t *self)
{
    GPIO_PinState level;
    level = (self->property.on_level == GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin(self->property.port, self->property.pin, level);
}

/**
 * @brief 熄灭 led
 */
static void off(led_t *self)
{
    GPIO_PinState level;
    level = (self->property.on_level == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;

    HAL_GPIO_WritePin(self->property.port, self->property.pin, level);
}

/**
 * @brief 切换 led 状态
 */
static void toggle(led_t *self)
{
    HAL_GPIO_TogglePin(self->property.port, self->property.pin);
}

/**
 * @brief 初始化 LED 驱动
 * 
 * @note GPIO 的初始化已经交给 CubeMX
 */
void led_init(led_t *self)
{
    if(!self){return;}

    if(self->ctx.is_init){return;}
    self->ctx.is_init = 1;

    self->on = on;
    self->off = off;
    self->toggle = toggle;
}
