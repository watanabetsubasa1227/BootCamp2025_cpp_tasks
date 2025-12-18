#pragma once

class LowPassFilter {
public:
    explicit LowPassFilter(double fc_hz = 1.0);

    void setCutoffHz(double fc_hz);
    void reset(double y0 = 0.0);

    // dt[sec] を与えて 1サンプル処理
    double process(double x, double dt_sec);

private:
    void updateT_();

    double fc_hz_ = 1.0; // cutoff [Hz]
    double T_     = 1.0; // time constant [s]  (T = 1/(2πfc))
    double y_     = 0.0; // state (vout)
};
