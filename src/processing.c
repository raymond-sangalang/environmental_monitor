#include <zephyr/kernel.h>

#include "processing.h"

/* Queue that carries processed data */
K_MSGQ_DEFINE(processed_msgq,
              sizeof(struct processed_data),
              10,
              4);

void processing_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    struct sensor_data sensor;
    struct processed_data processed;

    while (1) {

        /* Wait for a new sensor reading */
        k_msgq_get(&sensor_msgq, &sensor, K_FOREVER);

        processed.temperature = sensor.temperature;
        processed.humidity = sensor.humidity;
        processed.air_quality = sensor.air_quality;

        processed.temp_alert = (sensor.temperature > 27);

        /* Send to the logger */
        k_msgq_put(&processed_msgq, &processed, K_FOREVER);
    }
}