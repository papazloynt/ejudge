#include <iostream>
#include <vector>

enum Operation {
    inc,
    dbl,
    dec
};

// Считаю количество едениц в числе побитого
size_t BitSize(size_t number) {
    size_t counter_of_1 = 0;
    while (number > 0) {
        counter_of_1 += number & static_cast<size_t>(1);
        number >>= 1;
    }
    return counter_of_1;
}

std::vector<Operation> GreedAlgorithm(size_t number) {
    std::vector<Operation> operations;
    while (number > 1 && number != 3) {
        if (number % 2 == 0) { }
        else if (BitSize(number - 1) < BitSize(number + 1)) {
            operations.push_back(inc);
        } else {
            operations.push_back(dec);
            ++number;
        }
        number /= 2;
        operations.push_back(dbl);
    }
    if (number == 3) {
        operations.push_back(inc);
        operations.push_back(dbl);
    }
    operations.push_back(inc);
    return operations;
}

int main() {
    size_t input_number;
    std::cin >> input_number;
    auto res_ = GreedAlgorithm(input_number);

    for (auto it = res_.rbegin(); it != res_.rend(); ++it) {
        switch(*it) {
            case inc:
                std::cout << "inc" << std::endl;
                break;
            case dbl:
                std::cout << "dbl" << std::endl;
                break;
            case dec:
                std::cout << "dec" << std::endl;
                break;
            default:
                break;
        }
    }
    return 0;
}
