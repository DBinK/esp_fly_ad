#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// 角度环数据PID参数
float ROL_ANGLE_P = 0;
float ROL_ANGLE_I = 0;
float ROL_ANGLE_D = 0;

float PIT_ANGLE_P = 0;
float PIT_ANGLE_I = 0;
float PIT_ANGLE_D = 0;

float YAW_ANGLE_P = 0;
float YAW_ANGLE_I = 0;
float YAW_ANGLE_D = 0;

// 角速度环数据PID参数
float ROL_RATE_P = 0;
float ROL_RATE_I = 0;
float ROL_RATE_D = 0;

float PIT_RATE_P = 0;
float PIT_RATE_I = 0;
float PIT_RATE_D = 0;

float YAW_RATE_P = 0;
float YAW_RATE_I = 0;
float YAW_RATE_D = 0;

// 预定义结构体
typedef struct {
    float roll;
    float pitch;
    float yaw;
} TargetAngle;   // 目标角度

typedef struct {
    float roll;
    float pitch;
    float yaw;
} MeasureAngle;  // 测量角度

typedef struct {
    float roll;   // gx
    float pitch;  // gy
    float yaw;    // gz
} TargetRate;     // 目标角速度

typedef struct {
    float roll;   // gx
    float pitch;  // gy
    float yaw;    // gz
} MeasureRate;    // 测量角速度


#endif // CONFIG_H
