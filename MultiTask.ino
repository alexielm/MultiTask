#include <map>

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

#define LED_PIN      2   // Connected to D4

uint8_t newStack[1024];

std::map<String, int> dict;

int runningTasks = 0;
uint32_t runningTaskStacks[10];

void RunTask(void (*taskFunction)(), uint8_t* taskStack) {
    uint32_t oldStack = getStackPointer();

    uint32_t newStackTop = (uint32_t)taskStack + sizeof(newStack) - 4; 
    setStackPointer(newStackTop);

    taskFunction();

    setStackPointer(oldStack);
}

void NextTask() {

}

void TaskDelay(unsigned long delay) {
    unsigned long endTime = millis() + delay;
    while (millis() < endTime) {
        NextTask(); 
    }
}

void Task1() {
    while(true) {
      Serial.println("Task1 is executing");
      TaskDelay(750);
    }
}

void setup() {
    Serial.begin(115200);

    dict["key1"] = 100;
    dict["key2"] = 200;

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    RunTask(Task1, newStack);
}

void loop() {
    digitalWrite(LED_PIN, LOW); 
    TaskDelay(200);
    digitalWrite(LED_PIN, HIGH); 
    TaskDelay(200);
}
