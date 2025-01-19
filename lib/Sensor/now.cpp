#ifndef ESPNowReceiver_h
#define ESPNowReceiver_h

#include <Arduino.h>
#include <WiFi.h>
#include "ESPNowW.h"
#include <ArduinoJson.h>

class ESPNowReceiver {
public:
    ESPNowReceiver();
    void begin();
    void setRecvCallback(void (*callback)(const uint8_t *mac_addr, const uint8_t *data, int data_len));
    void getParsedData(int* dataArray, int& size); // 获取解析后的数据

private:
    static void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);
    uint8_t _mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // 广播地址
    int _parsedData[10]; // 存储解析后的数据
    int _dataSize;       // 解析后的数据大小
};

#endif