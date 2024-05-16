#include <Arduino.h>

extern "C" {
    uint32_t getStackPointer() {
        uint32_t sp;
        asm volatile("mov %0, a1" : "=r"(sp));
        return sp;
    }

    void setStackPointer(uint32_t sp) {
        asm volatile("mov a1, %0" :: "r"(sp));
    }
}

#define LED_PIN 2  // Connected to D4

uint8_t newStack1[1024];
uint8_t newStack2[1024];

struct Task {
    bool (*taskFunction)();
    uint32_t stackPointer;
};

Task runningTasks[10];
int taskCount = 1;
int currentTask = 0;

void RunTask(bool (*taskFunction)(), uint8_t* taskStack, int stackSize) {
    uint32_t newStackTop = (uint32_t)taskStack + stackSize - sizeof(uint32_t);

    *((uint32_t*)newStackTop) = (uint32_t)FirstRun;
    newStackTop -= sizeof(uint32_t) * 3;

    runningTasks[taskCount++] = {taskFunction, newStackTop};
}

void FirstRun() {
    Serial.println("Enter First Run");

    bool (*taskFunction)() = runningTasks[currentTask].taskFunction;
    while (taskFunction()) {
        NextTask();
    }

    delay(1000);
    Serial.println("-- Exiting ----");
    delay(1000);

    for (int i = 0; i < taskCount; ++i) {
        if (runningTasks[i].taskFunction == taskFunction) {
            for (int j = i; j < taskCount - 1; ++j) {
                runningTasks[j] = runningTasks[j + 1];
            }
            --taskCount;
            break;
        }
    }
    Serial.println("-- Removed ----");

    NextTask();
}

void NextTask() {
    runningTasks[currentTask].stackPointer = getStackPointer();
    currentTask = (currentTask + 1) % taskCount;
    uint32_t nextSP = runningTasks[currentTask].stackPointer;
    setStackPointer(nextSP);
}

void TaskDelay(unsigned long delay) {
    unsigned long endTime = millis() + delay;
    while (millis() < endTime) {
        NextTask();
    }
}

bool Task1() {
    while (true) {
        Serial.print(currentTask);
        Serial.println("-Main");
        TaskDelay(750);
    }
    return false;
}

bool Task2() {
    while (true) {
        Serial.print(currentTask);
        Serial.println("-Sub");
        TaskDelay(500);
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println();
    Serial.println();
    Serial.println("App started ----------------------");
    Serial.println();
    Serial.println();
    delay(100);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    RunTask(Task1, newStack1, 1024);
    RunTask(Task2, newStack2, 1024);
}

void loop() {
    digitalWrite(LED_PIN, LOW);
    TaskDelay(400);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("blink HIGH");
}
