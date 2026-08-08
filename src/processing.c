#include <zephyr/kernel.h>

#include "processing.h"
#include "config.h"


// Circular buffer 
static int temp_history[HISTORY_SIZE];  // = {0};

// Points to where the next reading will be stored - next position to write
static int history_index = 0;

// Number of valid samples currently stored 
static int sample_count = 0;

// Tracks the total sum of all the readings in the history buffer
static int running_sum = 0;


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

    // Holds the oldest temperature to be removed
    int old_temperature = temp_history[history_index];

    // Update the running sum of all the values
    if (sample_count == HISTORY_SIZE) {
        running_sum -= old_temperature;
    }
    // Store new temperature
    temp_history[history_index] = input->temperature; 

    // Add the new temperature to the running sum   
    running_sum += input->temperature;
    


    // Increase number of valid samples until history buffer reaches the HISTORY_SIZE
    if (sample_count < HISTORY_SIZE) {
        sample_count++;
    }

    // Advance the history index to next position in the circular buffer
    history_index = (history_index + 1) % HISTORY_SIZE;


    // Calculating the average using running sum
    output->average_temperature = running_sum / sample_count;


    // Calculating minimum and maximums
    // Initializing the min and max using first valid sample
    output->min_temperature = temp_history[0];
    output->max_temperature = temp_history[0];

    for (int i = 1; i < sample_count; i++) {

        if (temp_history[i] < output->min_temperature)
            output->min_temperature = temp_history[i];

        if (temp_history[i] > output->max_temperature)
            output->max_temperature = temp_history[i];
    }
}