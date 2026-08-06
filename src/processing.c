#include <zephyr/kernel.h>

#include "processing.h"
#include "config.h"


// #define HISTORY_SIZE 5

// Circular buffer 
static int temp_history[HISTORY_SIZE];  // = {0};

// Points to where the next reading will be stored - next position to write
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

    output->temp_alert = (input->temperature > TEMP_ALERT_THRESHOLD);


    // Store each new temperature
    temp_history[history_index] = input->temperature;
    
    // Advance the history index
    history_index = (history_index + 1) % HISTORY_SIZE;

    // Increase number of valid samples
    if (sample_count < HISTORY_SIZE) {
        sample_count++;
    }

    // Compute the average
    int sum = 0;

    // Initializing the min and max using first valid sample
    int min = temp_history[0];
    int max = temp_history[0];

    for (int i = 0; i < sample_count; i++) {
        int temp = temp_history[i];

        sum += temp;

        if (temp < min)
            min = temp;

        if (temp > max)
            max = temp;
    }

    output->average_temperature = sum / sample_count;
    output->min_temperature = min;
    output->max_temperature = max;
}