// MultiTask.h

#ifndef MULTITASK_H
#define MULTITASK_H

#include <Arduino.h>

void RunTask(void (*taskFunction)(), uint8_t* taskStack, int stackSize);
void TaskDelay(unsigned long delay);
void NextTask();

#endif // MULTITASK_H
