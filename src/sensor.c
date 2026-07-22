#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "sensor.h"

/***
 * sensor.c - source file in order to simulate the environmental sensors and produces data 
 ***/

LOG_MODULE_DECLARE(environment_monitor);

/* Queue definition */
K_MSGQ_DEFINE(sensor_msgq,
              sizeof(struct sensor_data),
              10,
              4);

void sensor_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    static int count = 0;

    while (1) {

        struct sensor_data data;

        data.temperature = 20 + (count % 10);
        data.humidity = 40 + ((count * 2) % 30);
        data.air_quality = 50 + ((count * 3) % 50);

        count++;

        k_msgq_put(&sensor_msgq, &data, K_NO_WAIT);

        k_sleep(K_SECONDS(2));
    }
}