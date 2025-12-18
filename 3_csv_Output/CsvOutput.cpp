#include <iostream>
#include <fstream>
#include <string>
#include "crlserial2025.hpp"

int main()
{
    crlSerial serial(TEXT("COM5"), 115200);

    if (!serial.check()) {
        std::cerr << "Error: serial port open failed." << std::endl;
        return 1;
    }

    std::ofstream ofs("joystick_log_mbedtime.csv");
    if (!ofs) {
        std::cerr << "Error: cannot open joystick_log_mbedtime.csv for write." << std::endl;
        return 1;
    }

    // time は mbed側のミリ秒と、秒の両方書いておくと便利
    ofs << "time_ms,time_s,sw,x,y\n";

    std::cout << "Start logging joystick data (mbed time in ms)..." << std::endl;

    const int NUM_SAMPLES = 100;

    for (int k = 0; k < NUM_SAMPLES; /* 中で進める */) {

        std::string buf[4];

        // mbed から「time_ms,sw,x,y」を受信
        int n = serial.read_sci(4, buf);
        if (n < 4) {
            continue;
        }

        try {
            // 文字列 → 数値に変換
            int    t_ms = std::stoi(buf[0]);          // mbed側の経過時間 [ms]
            int    sw   = std::stoi(buf[1]);
            int    x    = std::stoi(buf[2]);
            int    y    = std::stoi(buf[3]);
            double t_s  = t_ms / 1000.0;              // 秒に直したもの

            // （おまけ）PC側の時間も見たければこれも取れる
            double t_pc = serial.GetTime();

            // 画面表示
            std::cout << "mbed_t=" << t_s << " [s]  "
                      << "pc_t=" << t_pc << " [s]  "
                      << "SW=" << sw
                      << "  X=" << x
                      << "  Y=" << y
                      << std::endl;

            // CSVに time_ms と time_s 両方入れておく
            ofs << t_ms << "," << t_s << "," << sw << "," << x << "," << y << "\n";

            ++k;
        }
        catch (const std::exception& e) {
            std::cerr << "Parse error: " << e.what()
                      << " (tokens: \"" << buf[0] << "\", \"" << buf[1]
                      << "\", \"" << buf[2] << "\", \"" << buf[3] << "\")\n";
        }
    }

    ofs.close();
    std::cout << "Logging finished. Saved to joystick_log_mbedtime.csv" << std::endl;

    return 0;
}
