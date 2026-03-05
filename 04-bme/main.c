#include "stdio-task/stdio-task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task.h"
#include "bme280-driver.h"
#include "led-task/led-task.h"
#include "hardware/i2c.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

void rp2040_i2c_read(uint8_t* buffer, uint16_t length)
{
	i2c_read_timeout_us(i2c1, 0x76, buffer, length, false, 100000);
}

void rp2040_i2c_write(uint8_t* data, uint16_t size)
{
	i2c_write_timeout_us(i2c1, 0x76, data, size, false, 100000);
}

void version_callback(const char* args)
{
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char* args) {
    led_task_state_set(LED_STATE_ON);
}

void led_off_callback(const char* args) {
    led_task_state_set(LED_STATE_OFF);
}

void led_blink_callback(const char* args) {
    led_task_state_set(LED_STATE_BLINK);
}

void read_regs_callback(const char* args) {
    uint32_t addr = 0;
    uint32_t N = 0;

    // Парсим строку. Если пользователь ввел не 2 числа, выходим.
    if (sscanf(args, "%x %x", &addr, &N) != 2) {
        printf("Usage: read_regs <addr_hex> <N_dec>\n");
        return;
    }

    // Проверки безопасности
    if (addr > 0xFF || N > 0xFF || (addr + N) > 0x100) {
        printf("Error: Invalid address (0x%X) or count (%u)\n", addr, N);
        return;
    }

    // Выделяем буфер
    uint8_t buffer[256] = {0};

    // Вызываем чтение
    bme280_read_regs((uint8_t)addr, buffer, (uint8_t)N);

    // Выводим результат
    for (int i = 0; i < N; i++)
    {
    	printf("bme280 register [0x%X] = 0x%X\n", addr + i, buffer[i]);
    }
}

void write_reg_callback(const char* args)
{
    uint32_t addr = 0;
    uint32_t N = 0;

    if (sscanf(args, "%x %x", &addr, &N) != 2) {
        printf("Usage: read_regs <addr_hex> <N_dec>\n");
        return;
    }

    if (addr > 0xFF || N > 0xFF) {
        printf("Error: Invalid address (0x%X) or count (%u)\n", addr, N);
        return;
    }

    uint8_t data[2];
    data[0] = (uint8_t)addr;
    data[1] = (uint8_t)N;

    rp2040_i2c_write(data, 2);

    printf("Write: Register [0x%02X] set to 0x%02X\n", (uint8_t)addr, (uint8_t)N);
}

api_t device_api[] =
{
	{"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "light"},
    {"off", led_off_callback, "dark"},
    {"blink", led_blink_callback, "blink"},
    {"read_regs", read_regs_callback, "read regs"},
    {"write_reg", write_reg_callback, "write reg"},
	{NULL, NULL, NULL},
};

int main()
{
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init();

    i2c_init(i2c1, 100000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);

    bme280_init(rp2040_i2c_read, rp2040_i2c_write);

    while (1) {
        char* c = stdio_task_handle();
        protocol_task_handle(c);
    led_task_handle();
    }
}