#include "now.h"

ESPNowReceiver* ESPNowReceiver::instance = nullptr; // 声明静态成员变量

ESPNowReceiver::ESPNowReceiver() {  // 默认构造函数
    static uint8_t defaultMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 默认广播地址
    this->mac = defaultMac;
    this->parsedDataSize = 0;
    memset(lastMacAddr, 0, sizeof(lastMacAddr)); // 初始化最后接收到的MAC地址
}

ESPNowReceiver::ESPNowReceiver(uint8_t* mac) {
    this->mac = mac;
    this->parsedDataSize = 0;
    memset(lastMacAddr, 0, sizeof(lastMacAddr)); // 初始化最后接收到的MAC地址
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

    JsonDocument doc; 
    DeserializationError error = deserializeJson(doc, data);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    // 存储最后接收到的MAC地址
    memcpy(lastMacAddr, mac_addr, sizeof(lastMacAddr));

    parsedDataSize = doc.size();
    for (int i = 0; i < parsedDataSize && i < 10; i++) {
        parsedData[i] = doc[i].as<int>();
    }

    // serializeJsonPretty(doc, Serial);  // 格式化输出
}

int* ESPNowReceiver::getParsedData() {
    return parsedData;
}

int ESPNowReceiver::getParsedDataSize() {
    return parsedDataSize;
}

const uint8_t* ESPNowReceiver::getLastMacAddr() {
    return lastMacAddr;
}