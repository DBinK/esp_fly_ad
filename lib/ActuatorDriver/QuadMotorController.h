#ifndef QUADMOTORCONTROLLER_H
#define QUADMOTORCONTROLLER_H

#include "ESCController.h"

class QuadMotorController
{
public:
    QuadMotorController(int m1_pin, int m1_channel=1, int m2_pin, int m2_channel=2, int m3_pin, int m3_channel=3, int m4_pin, int m4_channel=4, int limit_min_thr = 0, int limit_max_thr = 1000);

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