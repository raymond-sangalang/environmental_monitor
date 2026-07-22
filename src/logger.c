#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "sensor.h"
/***
 * logger.c - Consumes sensor data and logs it
 * 
 ***/

LOG_MODULE_DECLARE(environment_monitor);

void logger_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    struct sensor_data data;

    while (1) {

        k_msgq_get(&sensor_msgq, &data, K_FOREVER);

        LOG_INF("Temp: %d C | Humidity: %d%% | Air Quality: %d",
                data.temperature,
                data.humidity,
                data.air_quality);
    }
}