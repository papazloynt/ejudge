#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv){
    std::string digit;
    long long sum = 0;

    std::ifstream in(argv[1], std::ios::in);
    if (in.is_open()) {
        while (std::getline(in, digit)) {
            if (!digit.empty()) {
                long long digit_ = std::strtoll(digit.c_str(), NULL, 10);
                sum += digit_ % 256;
            }
        }
    }
    in.close();

    std::ofstream out;
    out.open(argv[2]);
    if (out.is_open())
    {
        out << sum % 256;
    }
    out.close();
}
