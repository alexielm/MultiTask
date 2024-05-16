// TaskSystem.h

#ifndef TASK_SYSTEM_H
#define TASK_SYSTEM_H

#include <Arduino.h>

void RunTask(void (*taskFunction)(), uint8_t* taskStack, int stackSize);
void TaskDelay(unsigned long delay);
void NextTask();

#endif // TASK_SYSTEM_H
