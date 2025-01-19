#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>

// 一些工具代码

// 一阶低通滤波器类
// 选择 alpha 的建议：
// - 快速响应：较大 alpha (例如 0.8 或 0.9)
// - 强滤波效果：较小 alpha (例如 0.1 或 0.2)
// - 时间常数 τ = 1 / alpha
//   例如，τ = 10 个采样周期，alpha = 0.1
class LowPassFilter
{
public:
    LowPassFilter(float alpha) : alpha(alpha), filteredValue(0.0) {}
    void setAlpha(float alpha)
    {
        this->alpha = alpha;
    }

    float filter(float input)
    {
        filteredValue = alpha * input + (1.0 - alpha) * filteredValue;
        return filteredValue;
    }

private:
    float alpha;         // 滤波器系数，范围 0 到 1
    float filteredValue; // 滤波后的值

};

// 滑动平均滤波器类
class MovingAverageFilter
{
public:
    MovingAverageFilter(int windowSize) : windowSize(windowSize), bufferIndex(0), sum(0.0)
    {
        buffer = new float[windowSize];
        for (int i = 0; i < windowSize; i++)
        {
            buffer[i] = 0.0;
        }
    }

    ~MovingAverageFilter()
    {
        delete[] buffer;
    }

    float filter(float input)
    {
        sum -= buffer[bufferIndex];                   // 减去旧值
        buffer[bufferIndex] = input;                  // 存储新值
        sum += input;                                 // 加上新值
        bufferIndex = (bufferIndex + 1) % windowSize; // 更新索引
        return sum / windowSize;                      // 返回平均值
    }

private:
    int windowSize;
    int bufferIndex;
    float sum;
    float *buffer;
};

#endif // UTILITY_H