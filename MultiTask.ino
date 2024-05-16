#include <Arduino.h>
#include "TaskSystem.h"

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
    for (int i = 0; i < 50; i++) {
        Serial.print(i);
        Serial.println("Sub");
        TaskDelay(175275);
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
