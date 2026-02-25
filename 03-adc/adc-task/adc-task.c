#include "hardware/adc.h"

#include <stdio.h>
#include "pico/stdlib.h"

const uint ADC_PIN = 26;
uint ADC_CHANNEL = 0;
uint ADC_TEMP_PIN = 4;

void adc_task_init()
{
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_set_temp_sensor_enabled(true);
}

float adc_task_measure_voltage()
{
    	adc_select_input(ADC_CHANNEL);
        uint16_t voltage_counts = adc_read();
        float voltage = voltage_counts * 3.3f / 4096.0f;

        return voltage;
}

float adc_task_measure_temperature()
{
    adc_select_input(ADC_TEMP_PIN);
    uint16_t temp_counts = adc_read();
    float temp_V = temp_counts * 3.3f / 4096.0f;
    float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;

    return temp_C;
}
