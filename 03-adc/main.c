#include "stdio-task/stdio-task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "adc-task/adc-task.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

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

void get_adc_callback(const char* args) {
    float voltage_V = adc_task_measure_voltage();
    printf("%f\n", voltage_V);
}

void get_temp_callback(const char* args) {
    float temp_C = adc_task_measure_temperature();
    printf("%f\n", temp_C);
}

void tm_start_callback(const char* args) {
    adc_task_set_state(ADC_TASK_STATE_RUN);
    printf("tm start\n");
}

void tm_stop_callback(const char* args) {
    adc_task_set_state(ADC_TASK_STATE_IDLE);
    printf("tm stop\n");
}

api_t device_api[] =
{
	{"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "light"},
    {"off", led_off_callback, "dark"},
    {"blink", led_blink_callback, "blink"},
    {"get_adc", get_adc_callback, "measure voltage on GPIO 26"},
    {"get_temp", get_temp_callback, "measure temperature"},
    {"tm_start_callback", tm_start_callback, "start telemetry"},
    {"tm_stop_callback", tm_stop_callback, "stop telemetry"},
	{NULL, NULL, NULL},
};

int main()
{
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init();
    adc_task_init();
    while (1) {
        char* c = stdio_task_handle();
        protocol_task_handle(c);
    led_task_handle();
    }
}

