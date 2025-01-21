#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
public:
    void begin(USBCDC &serial, long baudRate = 115200);
    String readLine();
    bool UpdatePidParams(float &kp, float &ki, float &kd);
    String receivedData;
    String key;
    String valueStr;
    float value;

private:
    USBCDC* _serial;
};

#endif