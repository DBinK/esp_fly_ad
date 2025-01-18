#include "imu.h"

IMUClass::IMUClass(int sck, int miso, int mosi, int cs) 
    : customSPI(HSPI), 
      imuSensor(customSPI, cs), 
      CUSTOM_SPI_SCK(sck), 
      CUSTOM_SPI_MISO(miso), 
      CUSTOM_SPI_MOSI(mosi), 
      CUSTOM_SPI_CS(cs) {}

bool IMUClass::begin() {
    // 初始化自定义SPI
    customSPI.begin(CUSTOM_SPI_SCK, CUSTOM_SPI_MISO, CUSTOM_SPI_MOSI, CUSTOM_SPI_CS);
    
    // 开始与IMU通信
    int status = imuSensor.begin();
    if (status < 0) {
        return false; // 初始化失败
    }
    return true; // 初始化成功
}

void IMUClass::getData(float &accX, float &accY, float &accZ, float &gyrX, float &gyrY, float &gyrZ, float &temp) {
    // 读取传感器数据
    imuSensor.getAGT();
    
    // 获取加速度和角速度数据
    accX = imuSensor.accX();
    accY = imuSensor.accY();
    accZ = imuSensor.accZ();
    gyrX = imuSensor.gyrX();
    gyrY = imuSensor.gyrY();
    gyrZ = imuSensor.gyrZ();
    temp = imuSensor.temp();
}