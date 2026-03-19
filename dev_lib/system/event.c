#include "event.h"

typedef struct event {
    uint16_t type;
    int32_t argc;
    void *argv[EVENT_PARAM_MAX];
} event_t;

static event_t event_queue[EVENT_QUEUE_SIZE];
static volatile uint8_t head = 0, tail = 0;

static event_handler_t handler_table[EVENT_MAX] = {0};

/**
 * @brief 从事件队列获取事件
 * @param event 事件指针
 * @return uint8_t 1:成功 0:失败
 */
static uint8_t get(event_t *event)
{
    if(head == tail){return 0;}
    *event = event_queue[tail];
    tail = (tail + 1) % EVENT_QUEUE_SIZE;
    return 1;
}

/**
 * @brief 分发事件
 * @param event 事件指针
 */
static void dispatch(event_t *event)
{
    if(event->type < EVENT_MAX && handler_table[event->type])
    {
        handler_table[event->type](event->argc, event->argv);
    }
}

/**
 * @brief 设置事件处理函数
 * @param type 事件类型
 * @param handler 事件处理函数
 */
static void set(uint16_t type, event_handler_t handler)
{
    if(type < EVENT_MAX)
    {
        handler_table[type] = handler;
    }
}

/**
 * @brief 发布事件
 * @param type 事件类型
 * @param argc 事件参数数量
 * @param ... 事件参数列表
 */
static void post(uint16_t type, int32_t argc, ...)
{
    uint8_t next = (head + 1) % EVENT_QUEUE_SIZE;
    if(next != tail){return;}       // 队列已满，丢弃事件
    
    event_queue[head].type = type;
    event_queue[head].argc = (argc > EVENT_PARAM_MAX) ? EVENT_PARAM_MAX : argc;

    va_list args;
    va_start(args, argc);
    for(uint8_t i = 0; i < event_queue[head].argc; i++)
    {
        event_queue[head].argv[i] = va_arg(args, void *);
    }
    va_end(args);

    head = next;
}

/**
 * @brief 处理事件队列中的事件，调用对应的事件处理函数
 */
static void process(void)
{
    event_t event;
    while(get(&event))
    {
        dispatch(&event);
    }
}

// 事件操作接口
const event_ops_t event = {
    .set = set,
    .post = post,
    .process = process
};
