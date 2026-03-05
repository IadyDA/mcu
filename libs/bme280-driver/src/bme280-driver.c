#include "bme280-driver.h"

typedef struct
{
	bme280_i2c_read i2c_read;
	bme280_i2c_write i2c_write;
} bme280_ctx_t;

static bme280_ctx_t bme280_ctx = {0};

void bme280_init(bme280_i2c_read i2c_read, bme280_i2c_write i2c_write)
{
    bme280_ctx.i2c_read = i2c_read;
    bme280_ctx.i2c_write = i2c_write;
}

void bme280_read_regs(uint8_t start_reg_address, uint8_t* buffer, uint8_t length)
{
    uint8_t data[1] = {start_reg_address};
    bme280_ctx.i2c_write(data, sizeof(data));
	bme280_ctx.i2c_read(buffer, length);
}

// void rp2040_i2c_read(uint8_t* buffer, uint16_t length)
// {
// 	i2c_read_timeout_us(i2c1, 0x76, buffer, length, false, 100000);
// }

// void rp2040_i2c_write(uint8_t* data, uint16_t size)
// {
// 	i2c_write_timeout_us(i2c1, 0x76, data, size, false, 100000);
// }