#include <Arduino.h>
#include <WiFi.h>
#include "ESPNowW.h"
#include <ArduinoJson.h>

#include "imu.h"


uint8_t mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // 广播地址

void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
             mac_addr[5]);
    Serial.print("Last Packet Recv from: ");
    Serial.println(macStr);
    Serial.print("Last Packet Recv Data: ");

    Serial.printf("%s\n", data);

    // data 数据举例 String data = "[1, 110, 93, 116, 111, 8, 0, 6]"

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, data);

    if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
    }
    else{
        // 打印解析的数据
        Serial.println("解析的数据：");
        // for 循环遍历数组
        for (int i = 0; i < doc.size(); i++) {
            Serial.print(doc[i].as<int>());
            Serial.print(" ");
        }
        // serializeJsonPretty(doc, Serial);
        Serial.println();
    }
    

        
    // 翻转针脚电平
    pinMode(15, OUTPUT);
    digitalWrite(15, !digitalRead(15));
}

void setup() {
    Serial.begin(115200);
    Serial.println("ESPNow receiver Demo");
    WiFi.mode(WIFI_MODE_STA);
    ESPNow.set_mac(mac);
    WiFi.disconnect();
    ESPNow.init();
    ESPNow.reg_recv_cb(onRecv);
}

int i = 0;
void loop() {
    digitalWrite(15, LOW);
    Serial.printf("Loop %d\n", i++);
    delay(1000);
}