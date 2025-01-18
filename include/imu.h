#ifndef IMU_H
#define IMU_H

#include "ICM42688.h"
#include <SPI.h>

class IMUClass {
public:
    IMUClass(int sck = 3, int miso = 7, int mosi = 5, int cs = 9);
    bool begin();
    void getData();
    float getAccX() const;
    float getAccY() const;
    float getAccZ() const;
    float getGyrX() const;
    float getGyrY() const;
    float getGyrZ() const;
    float getTemp() const;
    
    float deltatUpdate();
    float getDeltat() const;

    float getPitch();
    float getRoll();
    float getYaw();


private:
    SPIClass customSPI;
    ICM42688 imuSensor;
    int CUSTOM_SPI_SCK;
    int CUSTOM_SPI_MISO;
    int CUSTOM_SPI_MOSI;
    int CUSTOM_SPI_CS;

    // 成员变量保存传感器数据
    float accX, accY, accZ;
    float gyrX, gyrY, gyrZ;
    float temp;

    // 保存姿态角
    float pitch, roll, yaw;

    // 时间相关变量
    unsigned long lastUpdate; // 上次更新时间
    float deltat; // 时间间隔
};

#endif // IMU_H