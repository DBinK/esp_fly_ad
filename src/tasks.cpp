#include <Arduino.h>

void Task1(void *pvParameters) {
    for (;;) {
        // 执行任务1的代码
        Serial.println("Task 1 is running");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 延迟1秒
    }
}

void Task2(void *pvParameters) {
    for (;;) {
        // 执行任务2的代码
        Serial.println("Task 2 is running");
        vTaskDelay(500 / portTICK_PERIOD_MS); // 延迟0.5秒
    }
}