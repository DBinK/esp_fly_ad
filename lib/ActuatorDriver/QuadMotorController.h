#ifndef QUADMOTORCONTROLLER_H
#define QUADMOTORCONTROLLER_H

#include "ESCController.h"

class QuadMotorController
{
public:
    QuadMotorController(int m1_pin = 18, int m2_pin = 16, int m3_pin = 21, int m4_pin = 17, int limit_min_thr = 0, int limit_max_thr = 1000);

    void setMotorsLimit(int limit_min_thr, int limit_max_thr);
    void setMotorsThr(int thr_list[]);
    void setMotorsThrRelative(int thr_list[]);
    void getMotorsThr(int thr_list[]);
    void reset();

private:
    ESCController motor_1;
    ESCController motor_2;
    ESCController motor_3;
    ESCController motor_4;
    ESCController *motors[4]; // 使用指针数组
};

#endif