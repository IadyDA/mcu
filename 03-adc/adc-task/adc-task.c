#include "hardware/adc.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "adc-task.h"

const uint ADC_PIN = 26;
uint ADC_CHANNEL = 0;
uint ADC_TEMP_PIN = 4;

static adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;
static uint64_t last_measure_ts = 0;
const uint64_t ADC_TASK_MEAS_PERIOD_US = 100000;

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

void adc_task_set_state(adc_task_state_t state)
{
    adc_state = state;
    if (adc_state == ADC_TASK_STATE_RUN) {
        last_measure_ts = time_us_64();
    }
}

void adc_task_handle()
{
    if (adc_state != ADC_TASK_STATE_RUN) {
        return;
    }

    uint64_t now = time_us_64();
    if (now >= last_measure_ts + ADC_TASK_MEAS_PERIOD_US) {
        last_measure_ts = now;
        
        float voltage_V = adc_task_measure_voltage();
        float temp_C = adc_task_measure_temperature();
        
        printf("%f %f\n", voltage_V, temp_C);
    }
}
