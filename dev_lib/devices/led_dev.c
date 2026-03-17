#include "led_dev.h"

led_t on_board_bule_led = {.property = {.port = ONBORAD_BULE_LED_GPIO_Port, .pin = ONBORAD_BULE_LED_Pin, .on_level = GPIO_PIN_RESET}};
led_t on_board_green_led = {.property = {.port = ONBORAD_GREEN_LED_GPIO_Port, .pin = ONBORAD_GREEN_LED_Pin, .on_level = GPIO_PIN_RESET}};

/**
 * @brief 初始化 led 实例
 */
void led_dev_init(void)
{
    led_init(&on_board_bule_led);
    led_init(&on_board_green_led);
}

/**
 * @brief led 实例进程函数
 */
void led_dev_proc(void)
{
    on_board_bule_led.toggle(&on_board_bule_led);
	// on_board_green_led.toggle(&on_board_green_led);
}
