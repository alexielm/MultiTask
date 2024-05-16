// MultiTask.cpp

#include "MultiTask.h"

namespace {
    struct Task {
        void (*taskFunction)();
        uint32_t stackPointer;
    };

    Task runningTasks[10];
    int taskCount = 1;
    int currentTask = 0;

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

    void TerminateCurrentTask() {
        delay(0);
        for (int i = currentTask; i < taskCount - 1; ++i) {
            runningTasks[i] = runningTasks[i + 1];
        }
        --taskCount;
        if (currentTask == taskCount) {
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

}

void RunTask(void (*taskFunction)(), uint8_t* taskStack, int stackSize) {
    uint32_t newStackTop = (uint32_t)taskStack + stackSize - sizeof(uint32_t);

    *((uint32_t*)newStackTop) = (uint32_t)FirstRun;
    newStackTop -= sizeof(uint32_t) * 3;

    runningTasks[taskCount++] = {taskFunction, newStackTop};
}

void TaskDelay(unsigned long delay) {
    unsigned long endTime = millis() + delay;
    while (millis() < endTime) {
        NextTask();
    }
}

void NextTask() {
    runningTasks[currentTask].stackPointer = getStackPointer();
    currentTask = (currentTask + 1) % taskCount;
    uint32_t nextSP = runningTasks[currentTask].stackPointer;
    setStackPointer(nextSP);
}
