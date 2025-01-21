#ifndef CONFIG_H
#define CONFIG_H


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
float ROL_RATE_P = 2.7;
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
    float roll  = 0;
    float pitch = 0;
    float yaw   = 0;
} TargetAngle;   // 目标角度

typedef struct {
    float roll  = 0;  // 右倾为负
    float pitch = 0;  // 前倾为负
    float yaw   = 0;  // 逆时针为负
} MeasureAngle;       // 测量角度

typedef struct {
    float roll  = 0;  // gx
    float pitch = 0;  // gy
    float yaw   = 0;  // gz
} TargetRate;         // 目标角速度

typedef struct {
    float roll  = 0;  // gx 右倾为负
    float pitch = 0;  // gy 前倾为负
    float yaw   = 0;  // gz 逆时针为负
} MeasureRate;        // 测量角速度

typedef struct {
    float roll  = 0;  // gx
    float pitch = 0;  // gy
    float yaw   = 0;  // gz
} OutRate;            // 输出角速度控制量

#endif // CONFIG_H

// void getData(int &data){
//     data = 10;
// }

// void getData(int* data){
//     _data = data;
//     *data = 10;
// }

// void getData(int *data){
//     _data = data;
//     *data = 10;
// }