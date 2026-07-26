#ifndef PROCESSING_H
#define PROCESSING_H

#include <zephyr/kernel.h>
#include <stdbool.h>

#include "sensor.h"

/* Data after processing */
struct processed_data {
    int temperature;
    int humidity;
    int air_quality;

    bool temp_alert;
};

/* Queue shared with the logger */
extern struct k_msgq processed_msgq;

/* Processing thread entry */
void processing_thread(void *, void *, void *);

/* Processing function -
    input points to the raw sensor data
    output points to storing result location */
void process_sensor_data(const struct sensor_data *input,
                         struct processed_data *output);

#endif