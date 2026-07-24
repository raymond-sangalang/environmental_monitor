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

#endif