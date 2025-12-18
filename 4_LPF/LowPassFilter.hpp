#include "LowPassFilter.h"
#include <cmath>
#include <iostream>
#include <algorithm>




namespace {
    constexpr double kPi = 3.14159265358979323846;
}

LowPassFilter::LowPassFilter(double fc_hz) : fc_hz_(fc_hz) {
    updateT_();
}

void LowPassFilter::setCutoffHz(double fc_hz) {
    fc_hz_ = std::max(fc_hz, 0.0);
    updateT_();
}

void LowPassFilter::reset(double y0) {
    y_ = y0;
}

void LowPassFilter::updateT_() {
    // T = 1/(2πfc)
    if (fc_hz_ <= 0.0) T_ = 1e30;               // fc=0 はほぼ変化しない扱い
    else               T_ = 1.0 / (2.0 * kPi * fc_hz_);
}

double LowPassFilter::process(double x, double dt_sec) {
    if (dt_sec <= 0.0) return y_;

    // スライド： y_next = y + (dt/T) * (x - y)
    double alpha = dt_sec / T_;
    alpha = std::clamp(alpha, 0.0, 1.0);        // 安全側（暴れ防止）
    y_ += alpha * (x - y_);
    return y_;
}

