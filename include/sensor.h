#ifndef SENSOR_H
#define SENSOR_H

#include <zephyr/kernel.h>

/***
 * sensor.h - Contains the shared data structure and queue declaration.
 *          - Defines the shared sensor_data type and declares the thread entry functions.
 * ***/

struct sensor_data {
    int temperature;
    int humidity;
    int air_quality;
};



// Message queue declaration 
extern struct k_msgq sensor_msgq;



// Thread entry functions 
void sensor_thread(void *, void *, void *);
// void logger_thread(void *, void *, void *);


#endif