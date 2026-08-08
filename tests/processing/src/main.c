#include <zephyr/kernel.h>
#include <zephyr/ztest.h>

#include "processing.h"
#include "config.h"

/*
 * processing.c expects sensor_msgq to exist.
 * Normally this is defined in sensor.c, but sensor.c
 * is not compiled into this unit test.
 */
K_MSGQ_DEFINE(sensor_msgq,
              sizeof(struct sensor_data),
              10,
              4);


/*
 * Test that a temperature above the configured threshold
 * triggers the high-temperature alert.
 */
ZTEST(processing, test_temperature_alert)
{
    struct sensor_data input = {
        .temperature = TEMP_ALERT_THRESHOLD + 1,
        .humidity = 50,
        .air_quality = 70
    };

    struct processed_data output;

    process_sensor_data(&input, &output);

    zassert_equal(output.temperature,
                  TEMP_ALERT_THRESHOLD + 1,
                  "Temperature mismatch");

    zassert_equal(output.humidity,
                  50,
                  "Humidity mismatch");

    zassert_equal(output.air_quality,
                  70,
                  "Air quality mismatch");

    zassert_true(output.temp_alert,
                 "Temperature alert should be active");
}


/*
 * Test the boundary condition.
 * Exactly the threshold should NOT trigger the alert
 * because production code uses > rather than >=.
 */
ZTEST(processing, test_temperature_alert_boundary)
{
    struct sensor_data input = {
        .temperature = TEMP_ALERT_THRESHOLD,
        .humidity = 50,
        .air_quality = 70
    };

    struct processed_data output;

    process_sensor_data(&input, &output);

    zassert_equal(output.temperature,
                  TEMP_ALERT_THRESHOLD,
                  "Temperature mismatch");

    zassert_false(output.temp_alert,
                  "Temperature alert should not be active at threshold");
}


/*
 * Register the test suite.
 */
ZTEST_SUITE(processing, NULL, NULL, NULL, NULL, NULL);


ZTEST(processing, test_temperature_sliding_window)
{
    processing_reset_history();

    struct processed_data output;
    struct sensor_data input = {
        .humidity = 50,
        .air_quality = 70
    };

    /*
     * Insert:
     * 20, 21, 22, 23, 24
     *
     * Average should be:
     * (20 + 21 + 22 + 23 + 24) / 5 = 22
     */
    for (int temperature = 20; temperature <= 24; temperature++) {
        input.temperature = temperature;
        process_sensor_data(&input, &output);
    }

    zassert_equal(output.average_temperature, 22,
                  "Average should be 22");

    /*
     * Add 25.
     *
     * The oldest value, 20, should be removed.
     *
     * New window:
     * 21, 22, 23, 24, 25
     *
     * Average = 23
     */
    input.temperature = 25;
    process_sensor_data(&input, &output);

    zassert_equal(output.average_temperature, 23,
                  "Sliding window average should be 23");
}


ZTEST(processing, test_temperature_min_max)
{
    processing_reset_history();

    struct processed_data output;
    struct sensor_data input = {
        .humidity = 50,
        .air_quality = 70
    };

    int temperatures[] = {25, 18, 30, 22, 27};

    for (int i = 0; i < HISTORY_SIZE; i++) {
        input.temperature = temperatures[i];
        process_sensor_data(&input, &output);
    }

    zassert_equal(output.min_temperature, 18,
                  "Minimum temperature should be 18");

    zassert_equal(output.max_temperature, 30,
                  "Maximum temperature should be 30");
}