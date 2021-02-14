#include "Processor.h"
#include <fstream>
constexpr char FILE_PATH[] = "Commands.txt";

int main() {
    Processor proc;
    std::ifstream fin(FILE_PATH);
    std::string command{};
    while (getline(fin, command)){
        std::cin.get();
        proc.ProcessCommand(command);
    }
    return 0;
}
