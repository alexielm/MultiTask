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

struct Task {
    void (*taskFunction)();
    uint32_t stackPointer;
};

Task runningTasks[10];
int taskCount = 1;
int currentTask = 0;

void RunTask(void (*taskFunction)(), uint8_t* taskStack, int stackSize) {
    uint32_t newStackTop = (uint32_t)taskStack + stackSize - sizeof(uint32_t);

    *((uint32_t*)newStackTop) = (uint32_t)FirstRun;
    newStackTop -= sizeof(uint32_t) * 3;

    runningTasks[taskCount++] = {taskFunction, newStackTop};
}

void TerminateCurrentTask() {
    delay(0);
    for (int i = currentTask; i < taskCount - 1; ++i) {
        runningTasks[i] = runningTasks[i + 1];
    }
    --taskCount;
    if(currentTask == taskCount) {
      currentTask = 0;
    }
    delay(0);
    uint32_t nextSP = runningTasks[currentTask].stackPointer;
    setStackPointer(nextSP);
}

void FirstRun() {
    void (*taskFunction)() = runningTasks[currentTask].taskFunction;
    taskFunction();
    NextTask();
    TerminateCurrentTask();
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

uint8_t newStack1[1024];
void Task1() {
    while (true) {
        Serial.println("Main");
        TaskDelay(1000);
    }
}

uint8_t newStack2[1024];
void Task2() {
    for(int i = 0; i < 5; i++) {
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
