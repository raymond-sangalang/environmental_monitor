#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


#include "sensor.h"
#include "processing.h"
#include "logger.h"
/***
 main.c - Start the application from main thread 
 ***/

LOG_MODULE_REGISTER(environment_monitor);

/* Sensor thread */
K_THREAD_STACK_DEFINE(sensor_stack, 1024);
struct k_thread sensor_thread_data;

#define PROCESSING_STACK_SIZE 1024

/* Processing thread */
K_THREAD_STACK_DEFINE(processing_stack, PROCESSING_STACK_SIZE);
struct k_thread processing_thread_data;

/* Logger thread */
K_THREAD_STACK_DEFINE(logger_stack, 1024);
struct k_thread logger_thread_data;



int main(void)
{
    LOG_INF("Environmental Monitor Started");

    k_thread_create(
        &sensor_thread_data,
        sensor_stack,
        K_THREAD_STACK_SIZEOF(sensor_stack),
        sensor_thread,
        NULL,
        NULL,
        NULL,
        5,
        0,
        K_NO_WAIT
    );

    k_thread_create(
        &processing_thread_data,
        processing_stack,
        K_THREAD_STACK_SIZEOF(processing_stack),
        processing_thread,
        NULL,
        NULL,
        NULL,
        5,
        0,
        K_NO_WAIT
    );

    k_thread_create(
        &logger_thread_data,
        logger_stack,
        K_THREAD_STACK_SIZEOF(logger_stack),
        logger_thread,
        NULL,
        NULL,
        NULL,
        5,
        0,
        K_NO_WAIT
    );


    while (1) {
        k_sleep(K_SECONDS(60));
    }

    return 0;
}