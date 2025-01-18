#include "imu.h"

IMUClass::IMUClass(int sck, int miso, int mosi, int cs)
    : customSPI(HSPI),
      imuSensor(customSPI, cs),
      CUSTOM_SPI_SCK(sck),
      CUSTOM_SPI_MISO(miso),
      CUSTOM_SPI_MOSI(mosi),
      CUSTOM_SPI_CS(cs),
      lastUpdate(0) {} // 初始化 lastUpdate

bool IMUClass::begin()
{
    // 初始化自定义SPI
    customSPI.begin(CUSTOM_SPI_SCK, CUSTOM_SPI_MISO, CUSTOM_SPI_MOSI, CUSTOM_SPI_CS);

    // 开始与IMU通信
    int status = imuSensor.begin();
    if (status < 0)
    {
        return false; // 初始化失败
    }
    lastUpdate = micros(); // 初始化 lastUpdate 为当前微秒时间
    return true;           // 初始化成功
}

void IMUClass::update()
{
    // 读取传感器数据
    imuSensor.getAGT();

    // 保存加速度和角速度数据到成员变量
    accX = imuSensor.accX();
    accY = imuSensor.accY();
    accZ = imuSensor.accZ();
    gyrX = imuSensor.gyrX();
    gyrY = imuSensor.gyrY();
    gyrZ = imuSensor.gyrZ();
    temp = imuSensor.temp();
}

void IMUClass::getAccData(float &ax, float &ay, float &az)
{
    ax = accX;  ay = accY;  az = accZ;  // 将数据保存到参数中
}

void IMUClass::getGyrData(float &gx, float &gy, float &gz)
{
    gx = gyrX;  gy = gyrY;  gz = gyrZ;  // 将数据保存到参数中
}

// 获取传感器数据
float IMUClass::getAccX() const { return accX; }
float IMUClass::getAccY() const { return accY; }
float IMUClass::getAccZ() const { return accZ; }
float IMUClass::getGyrX() const { return gyrX; }
float IMUClass::getGyrY() const { return gyrY; }
float IMUClass::getGyrZ() const { return gyrZ; }
float IMUClass::getTemp() const { return temp; }

// 计算时间间隔
float IMUClass::deltatUpdate()
{
    unsigned long Now = micros();
    float deltat = (Now - lastUpdate) / 1000000.0f; // 计算自上次更新以来的时间间隔（秒）
    lastUpdate = Now;                               // 更新 lastUpdate
    return deltat;                                  // 返回时间间隔, 单位秒
}

float IMUClass::getDeltat() const { return deltat; }

// 计算 pitch 和 roll
float IMUClass::getPitch()
{
    pitch = atan2(accY, accZ) * 180 / PI;
    return pitch;
}

float IMUClass::getRoll()
{
    roll = atan2(accX, accZ) * 180 / PI;
    return roll;
}

float IMUClass::getYaw()
{
    deltat = deltatUpdate(); // 更新时间间隔
    yaw += gyrZ * deltat;    // 陀螺仪数据单位是度每秒，需要乘以时间间隔（毫秒）得到角度变化
    if (yaw < 0)
        yaw += 360; // 保证 yaw 在 0 到 360 度之间
    if (yaw >= 360)
        yaw -= 360;
    return yaw;
}

void IMUClass::getPitchRollYaw(float &pitch, float &roll, float &yaw)
{
    pitch = getPitch();
    roll = getRoll();
    yaw = getYaw();
}