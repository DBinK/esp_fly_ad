#include <Arduino.h>
#include "QuadMotorController.h"

#include "imu.h"

IMUClass imu;  // 使用自定义引脚 IMUClass imu(10, 11, 12, 13);

// 定义电机引脚和通道
#define M1_PIN 18
#define M1_CHANNEL 0
#define M2_PIN 16
#define M2_CHANNEL 1
#define M3_PIN 21
#define M3_CHANNEL 2
#define M4_PIN 17
#define M4_CHANNEL 3

// 定义推力限制
#define LIMIT_MIN_THR 0
#define LIMIT_MAX_THR 1000

QuadMotorController quadMotorController(M1_PIN, M1_CHANNEL, M2_PIN, M2_CHANNEL, M3_PIN, M3_CHANNEL, M4_PIN, M4_CHANNEL, LIMIT_MIN_THR, LIMIT_MAX_THR);

void setup() {
    Serial.begin(115200);
    delay(1000); // 等待串口连接

    // 设置电机推力
    int thr_list[4] = {200, 300, 400, 500};
    quadMotorController.setMotorsThr(thr_list);

    // 获取并打印电机推力
    int thr_list_read[4];
    quadMotorController.getMotorsThr(thr_list_read);
    Serial.println("Motor Thrust Values:");
    for (int i = 0; i < 4; i++) {
        Serial.print("Motor ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(thr_list_read[i]);
    }

    // 设置相对推力
    int thr_relative_list[4] = {50, 100, 200, 400};
    quadMotorController.setMotorsThrRelative(thr_relative_list);

    // 获取并打印电机推力
    quadMotorController.getMotorsThr(thr_list_read);
    Serial.println("Motor Thrust Values after Relative Adjustment:");
    for (int i = 0; i < 4; i++) {
        Serial.print("Motor ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(thr_list_read[i]);
    }

    // 重置电机推力
    quadMotorController.reset();

    // 获取并打印电机推力
    quadMotorController.getMotorsThr(thr_list_read);
    Serial.println("Motor Thrust Values after Reset:");
    for (int i = 0; i < 4; i++) {
        Serial.print("Motor ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(thr_list_read[i]);
    }
}

void loop() {
    // 主循环中不需要执行任何操作
}