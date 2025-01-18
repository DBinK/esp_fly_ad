#ifndef IMU_H
#define IMU_H

#include "ICM42688.h"
#include <SPI.h>

class IMUClass {
public:
    IMUClass(int sck = 3, int miso = 7, int mosi = 5, int cs = 9);
    bool begin();
    void getData(float &accX, float &accY, float &accZ, float &gyrX, float &gyrY, float &gyrZ, float &temp);

private:
    SPIClass customSPI;
    ICM42688 imuSensor;
    int CUSTOM_SPI_SCK;
    int CUSTOM_SPI_MISO;
    int CUSTOM_SPI_MOSI;
    int CUSTOM_SPI_CS;
};

#endif // IMU_H