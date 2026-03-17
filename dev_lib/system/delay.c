#include "delay.h"

static uint32_t ticks_per_us;
static uint32_t last_cycle;
static uint64_t total_us;        // 64bit 单调时间

/**
 * @brief 初始化 dwt
 */
static void init(void)
{
    static uint8_t is_init = 0;
    if(is_init){return;}
    is_init = 1;
    
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;     // 开启 DWT
    DWT->CYCCNT = 0;     // 清零计数器
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;        // 开启 CYCCNT

    ticks_per_us = SystemCoreClock / 1000000UL;     // 计算每微秒 CPU 周期数
    last_cycle = DWT->CYCCNT;
    total_us = 0;
}

/** 
 * @brief 阻塞延迟 us
 * 
 * @param us 延时微秒数
 */
void delay_us(uint32_t us)
{
    init();

    volatile uint32_t start = DWT->CYCCNT;
    uint32_t tick = us * ticks_per_us;

    while(DWT->CYCCNT - start < tick);
}

/**
 * @brief 获取当前微秒 tick
 * 
 * @return 当前系统运行到现在的微秒数
 */
uint32_t delay_get_us_tick(void)
{
    init();

    uint32_t now = DWT->CYCCNT;
    uint32_t delta = now - last_cycle;       // 自动处理回绕
    last_cycle = now;

    total_us += delta / ticks_per_us;

    return (uint32_t)total_us;
}
