#include "imu.h"

IMUClass imu;  // 使用自定义引脚 IMUClass imu(10, 11, 12, 13);


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
    float accX, accY, accZ, gyrX, gyrY, gyrZ, temp;
    imu.getData(accX, accY, accZ, gyrX, gyrY, gyrZ, temp);
    
    // 显示数据
    Serial.printf("AccX: %f\tAccY: %f\tAccZ: %f\tGyrX: %f\tGyrY: %f\tGyrZ: %f\tTemp: %f\n", 
                  accX, accY, accZ, gyrX, gyrY, gyrZ, temp);
    delay(100);
}