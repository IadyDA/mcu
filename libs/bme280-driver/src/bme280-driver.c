#include "bme280-driver.h"
#include "bme280-regs.h"

typedef struct
{
	bme280_i2c_read i2c_read;
	bme280_i2c_write i2c_write;
} bme280_ctx_t;

static bme280_ctx_t bme280_ctx = {0};

typedef struct {
    uint16_t dig_T1; int16_t dig_T2; int16_t dig_T3;
    uint16_t dig_P1; int16_t dig_P2; int16_t dig_P3; int16_t dig_P4;
    int16_t dig_P5; int16_t dig_P6; int16_t dig_P7; int16_t dig_P8; int16_t dig_P9;
    uint8_t  dig_H1; int16_t dig_H2; uint8_t  dig_H3;
    int16_t dig_H4; int16_t dig_H5; int8_t  dig_H6;
    int32_t t_fine;
} bme280_calib_t;

static bme280_calib_t calib = {0};

void bme280_init(bme280_i2c_read i2c_read, bme280_i2c_write i2c_write)
{
    bme280_ctx.i2c_read = i2c_read;
    bme280_ctx.i2c_write = i2c_write;
    uint8_t id_reg_buf[1] = {0};

    bme280_read_regs(BME280_REG_ID, id_reg_buf, sizeof(id_reg_buf));
    if (id_reg_buf[0] != 0x60) {
        printf("BME280: Unexpected chip ID! Expected 0x60, got 0x%02x\n", id_reg_buf[0]);
    }
    
    uint8_t ctrl_hum_reg_value = 0;
    ctrl_hum_reg_value |= (0b001 << 0); // osrs_h[2:0] = oversampling 1
    bme280_write_reg(BME280_REG_CTRL_HUM, ctrl_hum_reg_value);

    uint8_t config_reg_value = 0;
    config_reg_value |= (0b0 << 0); // spi3w_en[0:0] = false
    config_reg_value |= (0b000 << 2); // filter[4:2] = Filter off
    config_reg_value |= (0b001 << 5); // t_sb[7:5] = 62.5 ms
    bme280_write_reg(BME280_REG_CONFIG, config_reg_value);

    uint8_t ctrl_meas_value = 0;
    ctrl_meas_value |= (0b11 << 0); // Normal mode
    ctrl_meas_value |= (0b001 << 5); // Temperature oversampling 1
    ctrl_meas_value |= (0b001 << 2); // Pressure oversampling 1
    bme280_write_reg(BME280_REG_CTRL_MEAS, ctrl_meas_value);

}

void bme280_read_regs(uint8_t start_reg_address, uint8_t* buffer, uint8_t length)
{
    uint8_t data[1] = {start_reg_address};
    bme280_ctx.i2c_write(data, sizeof(data));
	bme280_ctx.i2c_read(buffer, length);
}

void bme280_write_reg(uint8_t reg_address, uint8_t value)
{
    uint8_t data[2] = {reg_address, value};
    bme280_ctx.i2c_write(data, sizeof(data));
}

uint16_t bme280_read_temp_raw()
{
	uint8_t read[2] = {0};
	bme280_read_regs(BME280_REG_TEMP_MSB, read, sizeof(read));
	uint16_t value = ((uint16_t)read[0] << 8) | ((uint16_t)read[1]);
	return value;
}

uint16_t bme280_read_pres_raw()
{
    uint8_t read[2] = {0};
    bme280_read_regs(BME280_REG_PRESS_MSB, read, sizeof(read));
    uint16_t value = ((uint16_t)read[0] << 8) | ((uint16_t)read[1]);
	return value;
}
uint16_t bme280_read_hum_raw()
{
    uint8_t read[2] = {0};
    bme280_read_regs(BME280_REG_HUM_MSB, read, sizeof(read));
    uint16_t value = ((uint16_t)read[0] << 8) | ((uint16_t)read[1]);
	return value;
}

float temp_in_SI()
{

}

float pres_in_SI();
float hum_in_SI();

