#ifndef I2C_BUS_H
#define I2C_BUS_H

#include "stm32f4xx_hal.h"

typedef struct i2c_bus {
    void (*dma_read_bytes)(struct i2c_bus *self, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);
    void (*read_bytes)(struct i2c_bus *self, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);
    void (*write_bytes)(struct i2c_bus *self, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);
    uint8_t (*get_dma_rx_busy)(struct i2c_bus *self);

    // 上下文
    void * const ctx;
} i2c_bus_t;

extern i2c_bus_t i2c1;
extern i2c_bus_t i2c2;

void i2c_bus_init(i2c_bus_t *self);

#endif
