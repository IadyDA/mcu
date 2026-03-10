#include "stdio-task/stdio-task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

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

void led_blink_set_period_ms_callback(const char* args) {
    uint period_ms = 0;
    sscanf(args, "%u", &period_ms);

    if (period_ms == 0) {
        printf("ERROR: period_ms = 0");
        return;
    }

    led_task_set_blink_period_ms(period_ms);
}

void help_callback(const char* args);


api_t device_api[] =
{
	{"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "light"},
    {"off", led_off_callback, "dark"},
    {"blink", led_blink_callback, "blink"},
    {"set_period", led_blink_set_period_ms_callback, "set the blink period"},
    {"help", help_callback, "print commands description"},
	{NULL, NULL, NULL},
};

void help_callback(const char* args) {
    for (int i = 0; device_api[i].command_name != NULL; i++) {
        printf("Command '%s': '%s'\n", 
               device_api[i].command_name, 
               device_api[i].command_help);
    }
}

int main()
{
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init();
    while (1) {
        char* c = stdio_task_handle();
        protocol_task_handle(c);
    led_task_handle();
    }
}

