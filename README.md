# MultiTask

MultiTask is a cooperative multitasking library for ESP8266, enabling multiple tasks to run with stack switching. It manages task scheduling and execution seamlessly, allowing efficient task handling in resource-constrained environments.

## Features

- Cooperative multitasking for ESP8266
- Stack switching for multiple tasks
- Easy-to-use API for task management and delays

## Installation

1. Download the MultiTask library from [GitHub](https://github.com/alexielm/MultiTask).
2. Place the `MultiTask` folder in your Arduino `libraries` directory.
3. Restart the Arduino IDE.

## Usage

Include the MultiTask library in your sketch and use the provided functions to create and manage tasks.

### Functions

- `void RunTask(void (*taskFunction)(), uint8_t* taskStack, int stackSize);`
  - Adds a new task to be managed by the scheduler.
- `void TaskDelay(unsigned long delay);`
  - Delays the execution of the current task for the specified duration.
- `void NextTask();`
  - Switches to the next task in the scheduler.

### Example

Here's an example demonstrating how to use the MultiTask library:

```cpp
#include <Arduino.h>
#include "TaskSystem.h"

//== App Code ===

#define LED_PIN 2  // Connected to D4

uint8_t newStack1[1024];
void Task1() {
    while (true) {
        Serial.println("Main");
        TaskDelay(1000);
    }
}

uint8_t newStack2[1024];
void Task2() {
    for (int i = 0; i < 5; i++) {
        Serial.print(i);
        Serial.println("Sub");
        TaskDelay(275);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println("App started ----------------------");
    delay(500);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    RunTask(Task1, newStack1, sizeof(newStack1));
    RunTask(Task2, newStack2, sizeof(newStack2));
}

void loop() {
    digitalWrite(LED_PIN, LOW);
    TaskDelay(400);
    digitalWrite(LED_PIN, HIGH);
    TaskDelay(600);
}
