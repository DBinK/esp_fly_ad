#include <Arduino.h>
#include <SPI.h>

// 寄存器地址
#define ICM42688_DEVICE_CONFIG 0x11         // 设备配置寄存器地址
#define ICM42688_PWR_MGMT0 0x4E             // 电源管理寄存器地址
#define ICM42688_WHO_AM_I 0x75              // 设备ID寄存器地址
#define ICM42688_ACCEL_DATA_X1 0x1F         // 加速度计X轴数据寄存器地址（高8位）
#define ICM42688_ACCEL_DATA_X0 0x20         // 加速度计X轴数据寄存器地址（低8位）
#define ICM42688_GYRO_DATA_X1 0x25          // 陀螺仪X轴数据寄存器地址（高8位）
#define ICM42688_GYRO_DATA_X0 0x26          // 陀螺仪X轴数据寄存器地址（低8位）
#define ICM42688_REG_BANK_SEL 0x76          // 寄存器库选择地址
#define ICM42688_TEMP_DATA1 0x1D            // 温度数据寄存器地址（高8位）

// 配置用的位掩码
#define ICM42688_PWR_TEMP_ON (0 << 5)       // 电源管理：温度传感器开启
#define ICM42688_PWR_TEMP_OFF (1 << 5)      // 电源管理：温度传感器关闭
#define ICM42688_PWR_GYRO_MODE_LN (3 << 2)  // 电源管理：陀螺仪低噪声模式
#define ICM42688_PWR_ACCEL_MODE_LN (3 << 0) // 电源管理：加速度计低噪声模式

#define ICM42688_GFS_2000DPS (0x00 << 5)    // 陀螺仪满量程范围：2000度/秒
#define ICM42688_AFS_16G (0x00 << 5)        // 加速度计满量程范围：16g
#define ICM42688_GODR_1kHz 0x06             // 陀螺仪输出数据速率：1kHz
#define ICM42688_AODR_1kHz 0x06             // 加速度计输出数据速率：1kHz

#define SPI_TIMEOUT 500000  // SPI通信超时时间（500ms）


// 定义针脚
const int MOSI_PIN = 5;
const int MISO_PIN = 7;
const int SCLK_PIN = 3;
const int CS_PIN = 9;

int i = 0;
uint8_t whoAmI = 0;

// 创建 SPIClass 实例并初始化为 HSPI
SPIClass *spi = NULL;

static const int spiClk = 1000000;  // 1 MHz

void setup() {
  Serial.begin(115200);
  Serial.println("ICM42688 begin success!!!"); 
  spi = new SPIClass(HSPI);
  // 开始 SPI 并指定针脚
  spi->begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  // 配置 SPI 参数 (例如: 4 MHz, MSBFIRST, SPI_MODE0)
  pinMode(spi->pinSS(), OUTPUT);  // // 设置从设备选择引脚为输出模式，因为 Arduino API 不会自动拉低 SS 引脚
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
}


void loop() {
  i++;
  Serial.printf("开始运行: %d\n", i);  
  
  // 读取 WHO_AM_I 寄存器
  digitalWrite(CS_PIN, LOW);
  spi->transfer(ICM42688_WHO_AM_I | 0x80);
  whoAmI = spi->transfer(0x00);
  digitalWrite(CS_PIN, HIGH);

  Serial.printf("WHO_AM_I: %d\n", whoAmI);
  delay(1000);
}


void spiCommand(SPIClass *spi, byte data) {
  // 按照常规 Arduino SPI API 的方式使用
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(spi->pinSS(), LOW);  // 拉低 SS 引脚以准备传输
  spi->transfer(data);
  digitalWrite(spi->pinSS(), HIGH);  // 拉高 SS 引脚以表示传输结束
  spi->endTransaction();
}