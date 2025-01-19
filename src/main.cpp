#include <Arduino.h>
#include <QuickPID.h>

#include "imu.h"
#include "now.h"
#include "motors.h"
#include "config.h"
#include "utility.h"

// 创建数据结构体
TargetAngle tgAngle;
MeasureAngle msAngle;
TargetRate tgRate;
MeasureRate msRate;

// 全局变量
float deltat;

// 创建IMU对象
IMUClass imu;  // 使用自定义引脚 IMUClass imu(10, 11, 12, 13);

// 创建电机对象
QuadMotorController motors; // 使用自定义引脚 QuadMotorController motors(9, 8, 7, 6);

// 创建PID对象, 参数在 config.h 中定义
QuickPID pidRollAngle(&msAngle.roll, &tgAngle.roll, &tgRate.roll, 
                        ROL_ANGLE_P, ROL_ANGLE_I, ROL_ANGLE_D, QuickPID::Action::direct);
QuickPID pidPitchAngle(&msAngle.pitch, &tgAngle.pitch, &tgRate.pitch, 
                        PIT_ANGLE_P, PIT_ANGLE_I, PIT_ANGLE_D, QuickPID::Action::direct);
// QuickPID pidYawAngle(&msAngle.yaw, &tgAngle.yaw, &tgRate.yaw, 
//                         YAW_ANGLE_P, YAW_ANGLE_I, YAW_ANGLE_D, QuickPID::Action::direct);

QuickPID pidRollRate(&msRate.roll, &tgRate.roll, &tgRate.roll, 
                        ROL_RATE_P, ROL_RATE_I, ROL_RATE_D, QuickPID::Action::direct);
QuickPID pidPitchRate(&msRate.pitch, &tgRate.pitch, &tgRate.pitch, 
                        PIT_RATE_P, PIT_RATE_I, PIT_RATE_D, QuickPID::Action::direct);
QuickPID pidYawRate(&msRate.yaw, &tgRate.yaw, &tgRate.yaw, YAW_RATE_P, 
                        YAW_RATE_I, YAW_RATE_D, QuickPID::Action::direct);

LowPassFilter lpf(0.1);

void setup() {
    // 初始化串口
    Serial.begin(115200);
    while (!Serial) {}
    Serial.println("初始化串口成功...");
    
    // 初始化IMU
    if (!imu.begin()) {
        Serial.println("IMU initialization unsuccessful");
        while (1) {}
    }
    imu.calculateGyrBias(500, 3);  // 计算陀螺仪Z轴零偏
    Serial.println("初始化 ICM4268 成功...");

    // 初始化PID
    pidRollAngle.SetTunings(ROL_ANGLE_P, ROL_ANGLE_I, ROL_ANGLE_D);
    pidRollAngle.SetMode(1);  // 设置PID模式为自动控制
    pidPitchAngle.SetTunings(PIT_ANGLE_P, PIT_ANGLE_I, PIT_ANGLE_D);
    pidPitchAngle.SetMode(1);  
    // pidYawAngle.SetTunings(YAW_ANGLE_P, YAW_ANGLE_I, YAW_ANGLE_D);
    // pidYawAngle.SetMode(1);  
    
    pidRollRate.SetTunings(ROL_RATE_P, ROL_RATE_I, ROL_RATE_D);
    pidRollRate.SetMode(1);  
    pidPitchRate.SetTunings(PIT_RATE_P, PIT_RATE_I, PIT_RATE_D);
    pidPitchRate.SetMode(1);  
    pidYawRate.SetTunings(YAW_RATE_P, YAW_RATE_I, YAW_RATE_D);
    pidYawRate.SetMode(1);  

    // 初始化电机
    motors.reset(); // 重置电机
    // 设置电机推力
    int thr_list[4] = {200, 300, 400, 500};
    motors.setMotorsThr(thr_list);

    delay(1000);
}


void loop() {

    imu.update();
    
    // 显示数据
    // Serial.printf("AccX: %f\t AccY: %f\t AccZ: %f\t GyrX: %f\t GyrY: %f\t GyrZ: %f\t Temp: %f\n", 
    //              ax, ay, az, gx, gy, gz, temp);

    imu.getPitchRollYaw(msAngle.pitch, msAngle.roll, msAngle.yaw);  // 获取姿态角
    imu.getGyrData(msRate.pitch, msRate.roll, msRate.yaw);  // 获取角速度
    deltat = imu.getDeltat() * 1000;        // ms
    
    // 对于倒置的IMU，将pitch和roll加减180度, roll 取反
    msAngle.pitch = (msAngle.pitch > 0) ? (msAngle.pitch - 180) : (msAngle.pitch + 180);
    msAngle.roll  =  -((msAngle.roll > 0) ?  (msAngle.roll - 180) :  (msAngle.roll + 180));

    // 低通滤波
    float filteredRoll = lpf.filter(msAngle.roll);

    // Serial.printf("rate: Pitch: %.2f\t Roll: %.2f\t Yaw: %.2f\t deltat: %.2f ms \n", msRate.pitch, msRate.roll, msRate.yaw, deltat);
    Serial.printf("angle: Pitch: %.2f\t Roll: %.2f\t Yaw: %.2f\t deltat: %.2f ms \n", msAngle.pitch, filteredRoll, msAngle.yaw, deltat);

    
    int motorThrs[4] = {(int)msAngle.pitch*5, 10, 10, (int)msAngle.roll*5};
    motors.setMotorsThr(motorThrs); // 设置电机转速

    delay(10);
}