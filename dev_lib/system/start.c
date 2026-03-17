#include "start.h"

/**
 * @brief 任务列表
 */
static void task_list(void)
{
	static task_t led_dev_task;
	task_create(&led_dev_task, led_dev_proc, 1000);
}

/**
 * @brief 初始化所有模块
 */
void start_init_all(void)
{
	HAL_Delay(1000);

	led_dev_init();

	task_list();
}

/**
 * @brief 任务循环
 */
void start_loop(void)
{
	task_loop();
}
