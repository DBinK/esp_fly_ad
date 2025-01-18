#include "imu.h"
#include "SensorFusion.h" //SF

IMUClass imu;  // 使用自定义引脚 IMUClass imu(10, 11, 12, 13);
SF fusion;

float gx, gy, gz, ax, ay, az, temp;
float gx_rad, gy_rad, gz_rad;
float pitch, roll, yaw;
float deltat;

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    Serial.println("ICM42688 begin...");
    
    if (!imu.begin()) {
        Serial.println("IMU initialization unsuccessful");
        while (1) {}
    }
    
    Serial.println("ax,ay,az,gx,gy,gz,temp_C");
}


void loop() {

    imu.getData();
    
    // 显示数据
    // Serial.printf("AccX: %f\t AccY: %f\t AccZ: %f\t GyrX: %f\t GyrY: %f\t GyrZ: %f\t Temp: %f\n", 
    //              ax, ay, az, gx, gy, gz, temp);

    // deltat = fusion.deltatUpdate(); //this have to be done before calling the fusion update
    
    // // 把角速度转弧度制
    // gx_rad *= DEG_TO_RAD;
    // gy_rad *= DEG_TO_RAD;
    // gz_rad *= DEG_TO_RAD;

    // // 传感器融合
    // fusion.MahonyUpdate(gx_rad, gy_rad, gz_rad, ax, ay, az, deltat);  //mahony is suggested if there isn't the mag and the mcu is slow

    // pitch = fusion.getPitch();
    // roll = fusion.getRoll();    //you could also use getRollRadians() ecc
    // yaw = fusion.getYaw();

    pitch = imu.getPitch();
    roll = imu.getRoll();
    yaw = imu.getYaw();
    deltat = imu.getDeltat() * 1000; // ms

    Serial.printf("Pitch: %.2f\t Roll: %.2f\t Yaw: %.2f\t deltat: %f ms \n", pitch, roll, yaw, deltat);

    delay(10);
}