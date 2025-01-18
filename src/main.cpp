#include "imu.h"
#include "SensorFusion.h" //SF

IMUClass imu;  // 使用自定义引脚 IMUClass imu(10, 11, 12, 13);
SF fusion;

float gx, gy, gz, ax, ay, az, temp;
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
    imu.getData(ax, ay, az, gx, gy, gz, temp);
    
    // 显示数据
    Serial.printf("AccX: %f\t AccY: %f\t AccZ: %f\t GyrX: %f\t GyrY: %f\t GyrZ: %f\t Temp: %f\n", 
                  ax, ay, az, gx, gy, gz, temp);

    deltat = fusion.deltatUpdate(); //this have to be done before calling the fusion update
    //choose only one of these two:
    fusion.MahonyUpdate(gx, gy, gz, ax, ay, az, deltat);  //mahony is suggested if there isn't the mag and the mcu is slow

    pitch = fusion.getPitch();
    roll = fusion.getRoll();    //you could also use getRollRadians() ecc
    yaw = fusion.getYaw();

    Serial.printf("Pitch: %.2f\t Roll: %.2f\t Yaw: %.2f \n", pitch, roll, yaw);

    delay(100);
}