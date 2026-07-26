#include <zephyr/kernel.h>

#include "processing.h"


#define HISTORY_SIZE 5

// Circular buffer 
static int temp_history[HISTORY_SIZE] = {0};

// Points to where the next reading will be stored 
static int history_index = 0;

// Number of valid samples currently stored 
static int sample_count = 0;


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

        process_sensor_data(&sensor, &processed);

        /* Send to the logger */
        k_msgq_put(&processed_msgq, &processed, K_FOREVER);
    }
}

void process_sensor_data(const struct sensor_data *input,
                         struct processed_data *output)
{
    output->temperature = input->temperature;
    output->humidity = input->humidity;
    output->air_quality = input->air_quality;

    output->temp_alert = (input->temperature > 27);
}