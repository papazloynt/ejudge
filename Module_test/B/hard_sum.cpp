#include <iostream>
#include <string>
#include <string_view>

long long TakeSum(std::string_view str_sum) {
    long long sum = 0;
    std::string digit;
    while (!str_sum.empty()) {
        if ((str_sum.front() >= '0' && str_sum.front() <= '9') || (str_sum.front() == '-' && digit.empty())) {
            digit += str_sum.front();
            str_sum.remove_prefix(1);
        } else {
            if (!digit.empty()) {
                if (digit != "-") {
                    sum += std::stoi(digit);
                }
                digit.clear();

                if (reinterpret_cast<char>(str_sum.front()) == '-')  { digit = "-";}
                str_sum.remove_prefix(1);
            } else {str_sum.remove_prefix(1);}
        }
    }
    if (!digit.empty() && digit != "-") { sum += std::stoi(digit); }
    return sum;
}


int main() {
    std::string string_numbers;
    long long sum = 0;

    while (std::getline(std::cin, string_numbers)) {
        sum += TakeSum(string_numbers);
    }

    std::cout << sum << std::endl;
}


