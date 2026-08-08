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

    int average_temperature;

    int min_temperature;
    int max_temperature;

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

/* Sensor processing */
bool validate_sensor_data(const struct sensor_data *input);


/* Reset function for testing */
void processing_reset_history(void);


#endif