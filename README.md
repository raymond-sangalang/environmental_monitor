# Zephyr RTOS Environmental Monitor

A simulated IoT environmental monitoring system built with **Zephyr RTOS**. This project demonstrates real-time processing, inter-thread communication, and modular embedded software architecture using Zephyr message queues and threads.

The goal of this project is to build a firmware-style application that collects sensor data, processes it, and produces meaningful environmental information in a way that can later be extended to real hardware sensors and IoT connectivity.

## Features

Current features:

* Zephyr RTOS application running on `native_sim`
* Multi-threaded architecture
* Simulated environmental sensor data generation
* Message queue-based communication between threads
* Sensor data processing layer
* Temperature threshold alerts
* Structured logging using Zephyr logging subsystem

Planned features:

* Moving average filtering for sensor data
* Sensor data validation
* Unit testing with Zephyr `ztest`
* MQTT connectivity
* Real sensor hardware integration

---

# System Architecture

The application follows a producer-processing-consumer architecture:

```
                 +----------------+
                 | Sensor Thread  |
                 |                |
                 | Generate data  |
                 +-------+--------+
                         |
                         |
                    sensor_msgq
                         |
                         v
              +---------------------+
              | Processing Thread   |
              |                     |
              | Data transformation |
              | Alert detection     |
              +----------+----------+
                         |
                         |
                  processed_msgq
                         |
                         v
              +----------------+
              | Logger Thread  |
              |                |
              | Display output |
              +----------------+
```

## Thread Responsibilities

### Sensor Thread

Responsible for generating sensor readings.

Current simulated values:

* Temperature
* Humidity
* Air quality

The sensor thread sends raw readings through a Zephyr message queue.

---

### Processing Thread

Responsible for converting raw sensor readings into processed data.

Current processing:

* Copies sensor readings
* Detects high-temperature conditions

Example:

```c
temp_alert = temperature > 27;
```

The processing logic is separated into its own function to improve maintainability and allow future unit testing.

---

### Logger Thread

Consumes processed sensor data and displays environmental information using Zephyr logging.

Example output:

```
Temp: 28 C [HIGH] | Humidity: 56% | Air Quality: 74
```

---

# Project Structure

```
environment_monitor/
|
├── include/
│   ├── sensor.h
│   ├── processing.h
│   └── logger.h
|
├── src/
│   ├── main.c
│   ├── sensor.c
│   ├── processing.c
│   └── logger.c
|
├── CMakeLists.txt
├── prj.conf
└── README.md
```

---

# Technologies Used

* C
* Zephyr RTOS
* West build system
* CMake
* Linux development environment

Zephyr concepts demonstrated:

* Threads
* Kernel message queues
* Logging subsystem
* Kconfig configuration
* Native simulator target

---

# Building and Running

## Activate Zephyr Environment

```bash
source ~/zephyrproject/.venv/bin/activate
```

Navigate to the project:

```bash
cd ~/zephyrproject/projects/environment_monitor
```

Build for the native simulator:

```bash
west build -p always -b native_sim
```

Run:

```bash
west build -t run
```

---

# Example Output

```
*** Booting Zephyr OS ***

Environmental Monitor Started

Temp: 20 C | Humidity: 40% | Air Quality: 50
Temp: 21 C | Humidity: 42% | Air Quality: 53
Temp: 28 C [HIGH] | Humidity: 56% | Air Quality: 74
Temp: 29 C [HIGH] | Humidity: 58% | Air Quality: 77
```

---

# Development Roadmap

## Completed

* [x] Create Zephyr application
* [x] Configure native simulator target
* [x] Implement sensor thread
* [x] Implement logger thread
* [x] Add message queue communication
* [x] Add processing thread
* [x] Separate processing logic from RTOS thread

## In Progress

* [ ] Circular buffer for historical sensor readings
* [ ] Moving average temperature filtering

## Future

* [ ] Unit tests using Zephyr ztest
* [ ] Sensor driver integration
* [ ] MQTT IoT connectivity
* [ ] Cloud dashboard visualization

---

# Learning Goals

This project is designed to explore embedded systems concepts including:

* Real-time task scheduling
* Thread communication
* Data processing pipelines
* Memory-efficient data structures
* Firmware modularity
* Testing embedded software

The long-term goal is to evolve this project from a simulated RTOS application into a complete IoT environmental monitoring system.
