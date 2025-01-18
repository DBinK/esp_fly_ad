#include "QuadMotorController.h"

QuadMotorController::QuadMotorController(int m1_pin, int m1_channel, int m2_pin, int m2_channel, int m3_pin, int m3_channel, int m4_pin, int m4_channel, int limit_min_thr, int limit_max_thr)
    : motor_1(m1_pin, m1_channel), motor_2(m2_pin, m2_channel), motor_3(m3_pin, m3_channel), motor_4(m4_pin, m4_channel)
{
    motors[0] = &motor_1;
    motors[1] = &motor_2;
    motors[2] = &motor_3;
    motors[3] = &motor_4;
    setMotorsLimit(limit_min_thr, limit_max_thr);
    reset();
}

void QuadMotorController::setMotorsLimit(int limit_min_thr, int limit_max_thr)
{
    for (int i = 0; i < 4; i++)
    {
        motors[i]->setLimit(limit_min_thr, limit_max_thr);
    }
}

void QuadMotorController::setMotorsThr(int thr_list[])
{
    for (int i = 0; i < 4; i++)
    {
        motors[i]->setThr(thr_list[i]);
    }
}

void QuadMotorController::setMotorsThrRelative(int thr_list[])
{
    for (int i = 0; i < 4; i++)
    {
        motors[i]->setThrRelative(thr_list[i]);
    }
}

void QuadMotorController::getMotorsThr(int thr_list[])
{
    for (int i = 0; i < 4; i++)
    {
        thr_list[i] = motors[i]->getThr();
    }
}

void QuadMotorController::reset()
{
    for (int i = 0; i < 4; i++)
    {
        motors[i]->reset();
    }
}