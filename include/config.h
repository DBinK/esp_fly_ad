#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// 角度环数据PID参数
float ROL_ANGLE_P = 1;
float ROL_ANGLE_I = 0;
float ROL_ANGLE_D = 0;

float PIT_ANGLE_P = 1;
float PIT_ANGLE_I = 0;
float PIT_ANGLE_D = 0;

float YAW_ANGLE_P = 1;
float YAW_ANGLE_I = 0;
float YAW_ANGLE_D = 0;

// 角速度环数据PID参数
float ROL_RATE_P = 1;
float ROL_RATE_I = 0;
float ROL_RATE_D = 0;

float PIT_RATE_P = 1;
float PIT_RATE_I = 0;
float PIT_RATE_D = 0;

float YAW_RATE_P = 1;
float YAW_RATE_I = 0;
float YAW_RATE_D = 0;

// 预定义结构体
typedef struct {
    float roll;
    float pitch;
    float yaw;
} TargetAngle;   // 目标角度

typedef struct {
    float roll;  // 右倾为负
    float pitch; // 前倾为负
    float yaw;   // 逆时针为负
} MeasureAngle;  // 测量角度

typedef struct {
    float roll;   // gx
    float pitch;  // gy
    float yaw;    // gz
} TargetRate;     // 目标角速度

typedef struct {
    float roll;   // gx 右倾为负
    float pitch;  // gy 前倾为负
    float yaw;    // gz 逆时针为负
} MeasureRate;    // 测量角速度

typedef struct {
    float roll;   // gx
    float pitch;  // gy
    float yaw;    // gz
} OutRate;      // 三维向量

#endif // CONFIG_H
