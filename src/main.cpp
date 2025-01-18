#include "ICM42688.h"

// 定义自定义SPI引脚
#define CUSTOM_SPI_SCK 3
#define CUSTOM_SPI_MISO 7
#define CUSTOM_SPI_MOSI 5
#define CUSTOM_SPI_CS 9

// an ICM42688 object with the ICM42688 sensor on custom SPI bus and chip select pin
SPIClass customSPI(HSPI);
ICM42688 IMU(customSPI, CUSTOM_SPI_CS);

void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}
  Serial.println("ICM42688 begin...");

  // 初始化自定义SPI
  customSPI.begin(CUSTOM_SPI_SCK, CUSTOM_SPI_MISO, CUSTOM_SPI_MOSI, CUSTOM_SPI_CS);

  // start communication with IMU
  int status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  Serial.println("ax,ay,az,gx,gy,gz,temp_C");
}

void loop() {
  // read the sensor
  IMU.getAGT();
  // display the data
  Serial.print(IMU.accX(),6);
  Serial.print("\t");
  Serial.print(IMU.accY(),6);
  Serial.print("\t");
  Serial.print(IMU.accZ(),6);
  Serial.print("\t");
  Serial.print(IMU.gyrX(),6);
  Serial.print("\t");
  Serial.print(IMU.gyrY(),6);
  Serial.print("\t");
  Serial.print(IMU.gyrZ(),6);
  Serial.print("\t");
  Serial.println(IMU.temp(),6);
  delay(100);
}