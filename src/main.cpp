#include <Arduino.h>

int i = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("ICM42688 begin success!!!");
}

void loop() {
  i++;
  Serial.printf("开始运行: %d\n", i);
  delay(1000);
}