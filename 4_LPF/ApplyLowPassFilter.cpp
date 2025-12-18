#include "LowPassFilter.h"
#include "LowPassFilter.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

static std::string trim(const std::string& s) {
    size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
    return s.substr(b, e - b);
}

static bool parseTwoDoubles(const std::string& line, double& a, double& b) {
    // 形式: a,b
    std::string s = trim(line);
    if (s.empty()) return false;

    auto pos = s.find(',');
    if (pos == std::string::npos) return false;

    std::string sa = trim(s.substr(0, pos));
    std::string sb = trim(s.substr(pos + 1));

    char* e1 = nullptr;
    char* e2 = nullptr;
    a = std::strtod(sa.c_str(), &e1);
    b = std::strtod(sb.c_str(), &e2);
    return (e1 && *e1 == '\0' && e2 && *e2 == '\0');
}

int main(int argc, char** argv) {
    // 使い方:
    //   ApplyLowPassFilter [fc_Hz]
    // 例:
    //   ApplyLowPassFilter 5
    double fc = (argc >= 2) ? std::stod(argv[1]) : 5.0; // デフォルト 5Hz

    std::ifstream ifs("LPF.csv");
    if (!ifs) {
        std::cerr << "Error: cannot open LPF.csv\n";
        return 1;
    }

    std::vector<double> t, x;

    std::string line;
    bool firstLine = true;
    while (std::getline(ifs, line)) {
        line = trim(line);
        if (line.empty()) continue;

        double tt = 0.0, xx = 0.0;
        if (!parseTwoDoubles(line, tt, xx)) {
            // たぶんヘッダ（Time[s],Voltage[V]）なので無視
            if (firstLine) {
                firstLine = false;
                continue;
            }
            // 途中で壊れた行があってもスキップ
            continue;
        }
        firstLine = false;

        t.push_back(tt);
        x.push_back(xx);
    }

    if (x.size() < 2) {
        std::cerr << "Error: not enough data rows in LPF.csv\n";
        return 1;
    }

    // フィルタ準備
    LowPassFilter lpf(fc);
    lpf.reset(x[0]);

    std::ofstream ofs("LPF_out.csv");
    if (!ofs) {
        std::cerr << "Error: cannot open LPF_out.csv\n";
        return 1;
    }

    ofs << "Time[s],Voltage[V],LPF[V]\n";

    double y = x[0];
    ofs << t[0] << "," << x[0] << "," << y << "\n";

    for (size_t i = 1; i < x.size(); ++i) {
        double dt = t[i] - t[i - 1];
        if (dt <= 0.0) dt = 0.0; // 念のため（時間が逆なら更新しない）
        y = lpf.process(x[i], dt);
        ofs << t[i] << "," << x[i] << "," << y << "\n";
    }

    std::cout << "Done. Saved to LPF_out.csv (fc=" << fc << " Hz)\n";
    return 0;
}
