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
OutRate outRate;  // 输出角速度控制信号

// 全局变量
float deltat;

// 创建IMU对象
IMUClass imu; // 使用自定义引脚 IMUClass imu(10, 11, 12, 13);

// 创建NOW对象
static uint8_t Mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 默认广播地址
ESPNowReceiver receiver(Mac); 

// 创建电机对象
QuadMotorController motors; // 使用自定义引脚 QuadMotorController motors(9, 8, 7, 6);

// 创建PID对象, 参数在 config.h 中定义, QuickPID(float *Input, float *Output, float *Setpoint);
QuickPID pidRollAngle(&msAngle.roll, &tgAngle.roll, &tgRate.roll,
                      ROL_ANGLE_P, ROL_ANGLE_I, ROL_ANGLE_D, QuickPID::Action::direct);
QuickPID pidPitchAngle(&msAngle.pitch, &tgAngle.pitch, &tgRate.pitch,
                       PIT_ANGLE_P, PIT_ANGLE_I, PIT_ANGLE_D, QuickPID::Action::direct);
// QuickPID pidYawAngle(&msAngle.yaw, &tgAngle.yaw, &tgRate.yaw,
//                         YAW_ANGLE_P, YAW_ANGLE_I, YAW_ANGLE_D, QuickPID::Action::direct);

QuickPID pidRollRate(&msRate.roll, &tgRate.roll, &outRate.roll,
                     ROL_RATE_P, ROL_RATE_I, ROL_RATE_D, QuickPID::Action::direct);
QuickPID pidPitchRate(&msRate.pitch, &tgRate.pitch, &outRate.pitch,
                      PIT_RATE_P, PIT_RATE_I, PIT_RATE_D, QuickPID::Action::direct);
QuickPID pidYawRate(&msRate.yaw, &tgRate.yaw, &outRate.yaw,
                    YAW_RATE_P, YAW_RATE_I, YAW_RATE_D, QuickPID::Action::direct);

LowPassFilter lpf(0.3);  // 低通滤波器, 参数越小, 滤波效果越好, 响应时间越慢

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    while (!Serial)
    {
    }
    Serial.println("初始化串口成功...");

    // 初始化IMU
    if (!imu.begin())
    {
        Serial.println("IMU initialization unsuccessful");
        while (1)
        {
        }
    }
    imu.calculateGyrBias(500, 3); // 计算陀螺仪Z轴零偏
    Serial.println("初始化 ICM4268 成功...");

    // 初始化NOW
    receiver.begin();

    // 初始化PID
    pidRollAngle.SetTunings(ROL_ANGLE_P, ROL_ANGLE_I, ROL_ANGLE_D);
    pidRollAngle.SetOutputLimits(-1000, 1000);  // 设置输出限幅
    pidRollAngle.SetMode(1);                    // 设置PID模式为自动控制
    pidPitchAngle.SetTunings(PIT_ANGLE_P, PIT_ANGLE_I, PIT_ANGLE_D);
    pidPitchAngle.SetOutputLimits(-1000, 1000);
    pidPitchAngle.SetMode(1);
    // pidYawAngle.SetTunings(YAW_ANGLE_P, YAW_ANGLE_I, YAW_ANGLE_D);
    // pidYawAngle.SetOutputLimits(-1000, 1000);
    // pidYawAngle.SetMode(1);

    pidRollRate.SetTunings(ROL_RATE_P, ROL_RATE_I, ROL_RATE_D);
    pidRollRate.SetOutputLimits(-1000, 1000);
    pidRollRate.SetMode(1);
    pidPitchRate.SetTunings(PIT_RATE_P, PIT_RATE_I, PIT_RATE_D);
    pidPitchRate.SetOutputLimits(-1000, 1000);
    pidPitchRate.SetMode(1);
    pidYawRate.SetTunings(YAW_RATE_P, YAW_RATE_I, YAW_RATE_D);
    pidYawRate.SetOutputLimits(-1000, 1000);
    pidYawRate.SetMode(1);

    // 初始化电机
    motors.reset(); // 重置电机
}

static TickType_t xLastWakeTime = 0; // 用于 vTaskDelayUntil 的变量
void loop()
{
    int* parsedData = receiver.getParsedDataFix();
    int parsedDataSize = receiver.getParsedDataSize();
    const uint8_t* lastMacAddr = receiver.getLastMacAddr();

    // 打印解析后的数据
    Serial.print("now Data: ");
    for (int i = 0; i < parsedDataSize; i++) {
        Serial.print(parsedData[i]);
        Serial.print(" ");
    }
    Serial.println();

    imu.update();

    // 显示数据
    // Serial.printf("AccX: %f\t AccY: %f\t AccZ: %f\t GyrX: %f\t GyrY: %f\t GyrZ: %f\t Temp: %f\n",
    //              ax, ay, az, gx, gy, gz, temp);

    imu.getPitchRollYaw(msAngle.pitch, msAngle.roll, msAngle.yaw); // 获取姿态角
    imu.getGyrData(msRate.pitch, msRate.roll, msRate.yaw);         // 获取角速度
    deltat = imu.getDeltat() * 1000;                               // ms

    // 对于倒置的IMU，将pitch和roll加减180度, roll 取反
    msAngle.pitch = (msAngle.pitch > 0) ? (msAngle.pitch - 180) : (msAngle.pitch + 180);
    msAngle.roll = -((msAngle.roll > 0) ? (msAngle.roll - 180) : (msAngle.roll + 180));

    // 低通滤波
    msAngle.pitch = lpf.filter(msAngle.pitch);
    msAngle.roll  = lpf.filter(msAngle.roll);
    msAngle.yaw   = lpf.filter(msAngle.yaw);

    // Serial.printf("rate: Roll: %.2f\t Pitch: %.2f\t Yaw: %.2f\t deltat: %.2f ms \n", 
    //              msRate.roll, msRate.pitch, msRate.yaw, deltat);
    Serial.printf("angle: Roll: %.2f\t Pitch: %.2f\t Yaw: %.2f\t deltat: %.6f ms \n",
                  msAngle.roll, msAngle.pitch, msAngle.yaw, deltat);

    // PID 运算
    pidRollAngle.Compute();
    pidPitchAngle.Compute();
    // pidYawAngle.Compute();
    pidRollRate.Compute();
    pidPitchRate.Compute();
    pidYawRate.Compute();

    // PID 输出
    Serial.printf(" tgRate: Roll: %.2f\t Pitch: %.2f\t Yaw: %.2f\t deltat: %.6f ms \n", 
                    tgRate.roll, tgRate.pitch, tgRate.yaw, deltat);
    // Serial.printf("outRate: Roll: %.2f\t Pitch: %.2f\t Yaw: %.2f\t deltat: %.2f ms \n", 
    //              outRate.roll, outRate.pitch, outRate.yaw, deltat);
    
    // 电机控制
    int rc_thr = 100;
    int motor1 = rc_thr + outRate.roll - outRate.pitch - outRate.yaw;
    int motor2 = rc_thr - outRate.roll - outRate.pitch + outRate.yaw;
    int motor3 = rc_thr - outRate.roll + outRate.pitch - outRate.yaw;
    int motor4 = rc_thr + outRate.roll + outRate.pitch + outRate.yaw;

    Serial.printf("motors: %d %d %d %d  rc_thr: %d\n", motor1, motor2, motor3, motor4, rc_thr);

    motors.setMotorsThr(motor1, motor2, motor3, motor4); // 设置电机推力


    if (xLastWakeTime == 0)  // 修正 vTaskDelayUntil 的使用
    {
        xLastWakeTime = xTaskGetTickCount();
    }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10)); // 10ms
}