#include <iostream>
#include <fstream>
/*----------------------------------------------------------------------*/
    //Todo 課題2 csvファイルを読み込んで，ターミナルに表示してください．
    // 読み込むcsvファイルは"LPF.csv"とします．
    // assets/直下にあるので使ってください．

int main() {
    std::ifstream file("LPF.csv");


    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    return 0;
}