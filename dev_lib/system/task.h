#ifndef TASK_H
#define TASK_H

#include "stm32f4xx_hal.h"

#define get_ms_tick       HAL_GetTick		// 不同平台修改成对应的获取毫秒的函数

typedef struct task {
	uint32_t last_time;
	uint32_t loop_time;
	void (*task_callback)(void);     // 进程函数回调
	struct task *p_next;
} task_t;

void task_create(task_t *new_task, void (*task_callback)(void), uint32_t loop_time);
void task_loop(void);

#endif
