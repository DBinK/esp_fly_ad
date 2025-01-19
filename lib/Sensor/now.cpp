#include "now.h"


// 将给定的值映射到给定的目标范围
float map_value_now(float value, float original_min, float original_max, float target_min, float target_max)
{
    return target_min + (value - original_min) * (target_max - target_min) / (original_max - original_min);
}

ESPNowReceiver* ESPNowReceiver::instance = nullptr; // 声明静态成员变量

ESPNowReceiver::ESPNowReceiver() {  // 默认构造函数
    static uint8_t defaultMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 默认广播地址
    this->mac = defaultMac;
    this->parsedDataSize = 0;
    memset(lastMacAddr, 0, sizeof(lastMacAddr)); // 初始化最后接收到的MAC地址
    lastReceiveTime = 0; // 初始化最后接收到数据的时间
    timer = NULL; // 初始化定时器指针
}

ESPNowReceiver::ESPNowReceiver(uint8_t* mac) {
    this->mac = mac;
    this->parsedDataSize = 0;
    memset(lastMacAddr, 0, sizeof(lastMacAddr)); // 初始化最后接收到的MAC地址
    lastReceiveTime = 0; // 初始化最后接收到数据的时间
    timer = NULL; // 初始化定时器指针
}

void ESPNowReceiver::begin() {
    Serial.begin(115200);
    Serial.println("ESPNow Receiver Initialized");
    WiFi.mode(WIFI_MODE_STA);
    ESPNow.set_mac(mac);
    WiFi.disconnect();
    ESPNow.init();
    ESPNow.reg_recv_cb(onReceiveStatic); // 注册静态成员函数作为回调
    instance = this; // 设置实例指针

    // 初始化定时器
    timer = timerBegin(0, 80, true); // 使用定时器0，分频系数80，计数器向上计数
    timerAttachInterrupt(timer, &ESPNowReceiver::timerCallback, true); // 将回调函数附加到定时器
    timerAlarmWrite(timer, 1000000, true); // 设置定时器中断周期为1秒
    timerAlarmEnable(timer); // 启动定时器中断
}

// 静态成员函数作为回调
void ESPNowReceiver::onReceiveStatic(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
    if (instance == nullptr) {
        Serial.println("ESPNowReceiver instance is not set!");
        return;
    }

    instance->onReceive(mac_addr, data, data_len);
}

void ESPNowReceiver::onReceive(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    JsonDocument doc; // 创建 JSON 文档
    DeserializationError error = deserializeJson(doc, data);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    // 存储最后接收到的MAC地址
    memcpy(lastMacAddr, mac_addr, sizeof(lastMacAddr));

    // 更新最后接收到数据的时间
    lastReceiveTime = millis();

    parsedDataSize = doc.size();
    for (int i = 0; i < parsedDataSize && i < 10; i++) {
        parsedData[i] = doc[i].as<int>();
    }

    // serializeJsonPretty(doc, Serial);  // 格式化输出
}

int* ESPNowReceiver::getParsedData() {
    return parsedData;
}
int* ESPNowReceiver::getParsedDataFix(){
    if (parsedData[0] != 0) {
        // 减去校准偏移值, 并映射到 -127 到 127
        parsedData[1] = (int)map_value_now(parsedData[1] - OFFSET_ly, 0, 255, -127, 127);
        parsedData[2] = (int)map_value_now(parsedData[2] - OFFSET_lx, 0, 255, -127, 127);
        parsedData[4] = (int)map_value_now(parsedData[4] - OFFSET_ry, 0, 255, -127, 127);
        parsedData[3] = (int)map_value_now(parsedData[3] - OFFSET_rx, 0, 255, -127, 127);

        Serial.printf("摇杆映射后数据: lx=%d, ly=%d, rx=%d, ry=%d\n", parsedData[2], parsedData[1], parsedData[3], parsedData[4]);
    }
    return parsedData;
}

int ESPNowReceiver::getParsedDataSize() {
    return parsedDataSize;
}

const uint8_t* ESPNowReceiver::getLastMacAddr() {
    return lastMacAddr;
}

bool ESPNowReceiver::isSignalLost() {
    unsigned long currentTime = millis();
    if (currentTime - lastReceiveTime < signalTimeout) {
        return false;
    }
    else {
        memset(parsedData, 0, sizeof(parsedData)); // 信号丢失，归零数据
        return true;
    }
}

// 定时器回调函数
void IRAM_ATTR ESPNowReceiver::timerCallback() {
    if (instance != nullptr) {
        instance->isSignalLost();
        Serial.println("timerCallback test");
    }
}