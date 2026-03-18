#include "i2c_bus.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

typedef struct i2c_bus_ctx {
    I2C_HandleTypeDef *handle;
    uint8_t is_init;
    volatile uint8_t dma_rx_busy;
} i2c_bus_ctx_t;

// i2c1 实例
static i2c_bus_ctx_t i2c1_ctx = {.handle = &hi2c1};
i2c_bus_t i2c1 = {.ctx = &i2c1_ctx};

// i2c2 实例
static i2c_bus_ctx_t i2c2_ctx = {.handle = &hi2c2};
i2c_bus_t i2c2 = {.ctx = &i2c2_ctx};

/**
 * @brief i2c dma 连续读
 */
static void dma_read_bytes(i2c_bus_t *self, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len)
{
    i2c_bus_ctx_t *p = (i2c_bus_ctx_t *)self->ctx;

    if(HAL_I2C_GetState(p->handle) != HAL_I2C_STATE_READY){return;}
    if(p->dma_rx_busy){return;}

    if(HAL_I2C_Mem_Read_DMA(p->handle, addr << 1, reg, I2C_MEMADD_SIZE_8BIT, buf, len) == HAL_OK)
    {
        p->dma_rx_busy = 1;
    }
}

/**
 * @brief i2c 连续读
 */
static void read_bytes(i2c_bus_t *self, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len)
{
    i2c_bus_ctx_t *p = (i2c_bus_ctx_t *)self->ctx;
    
    if(p->dma_rx_busy){return;}

	HAL_I2C_Mem_Read(p->handle, addr << 1, reg, I2C_MEMADD_SIZE_8BIT, buf, len, HAL_MAX_DELAY);
}

/**
 * @brief i2c 连续写
 */
static void write_bytes(i2c_bus_t *self, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len)
{
    i2c_bus_ctx_t *p = (i2c_bus_ctx_t *)self->ctx;
    
    if(p->dma_rx_busy){return;}

	HAL_I2C_Mem_Write(p->handle, addr << 1, reg, I2C_MEMADD_SIZE_8BIT, buf, len, HAL_MAX_DELAY);
}

/**
 * @brief 获取 i2c dma 状态
 * 
 * @return 1 为 dma 繁忙，0 为 dma 空闲
 */
static uint8_t get_dma_rx_busy(i2c_bus_t *self)
{
    i2c_bus_ctx_t *p = (i2c_bus_ctx_t *)self->ctx;

    return p->dma_rx_busy;
}

/**
 * @brief 初始 i2c 驱动
 */
void i2c_bus_init(i2c_bus_t *self)
{
    if(!self){return;}

    i2c_bus_ctx_t *p = (i2c_bus_ctx_t *)self->ctx;
    if(p->is_init){return;}
    p->is_init = 1;

    self->dma_read_bytes = dma_read_bytes;
    self->read_bytes = read_bytes;
    self->write_bytes = write_bytes;
    self->get_dma_rx_busy = get_dma_rx_busy;
}

/**
 * @brief i2c dma 接收完成回调
 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    i2c_bus_t *p = NULL;

    if(hi2c->Instance == I2C1){p = &i2c1;}
    else if(hi2c->Instance == I2C2){p = &i2c2;}

    if(!p){return;}
    i2c_bus_ctx_t *p_ctx = (i2c_bus_ctx_t *)p->ctx;

    p_ctx->dma_rx_busy = 0;
}

/**
 * @brief i2c dma 接收错误回调
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    i2c_bus_t *p = NULL;

    if(hi2c->Instance == I2C1){p = &i2c1;}
    else if(hi2c->Instance == I2C2){p = &i2c2;}

    if(!p){return;}
    i2c_bus_ctx_t *p_ctx = (i2c_bus_ctx_t *)p->ctx;

    p_ctx->dma_rx_busy = 0;
}
