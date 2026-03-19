#ifndef EVENT_H
#define EVENT_H

#include "stm32f4xx_hal.h"
#include <stdarg.h>

#define EVENT_QUEUE_SIZE        32
#define EVENT_MAX               32
#define EVENT_PARAM_MAX         8

typedef struct event {
    uint16_t type;
    int32_t argc;
    void *argv[EVENT_PARAM_MAX];
} event_t;

typedef void (*event_handler_t)(int32_t argc, void *argv[]);

typedef struct event_ops {
    void (*set)(uint16_t type, event_handler_t handler);
    void (*post)(uint16_t type, int32_t argc, ...);
    void (*process)(void);
} event_ops_t;

extern const event_ops_t event;

#endif
