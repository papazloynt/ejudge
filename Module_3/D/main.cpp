#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

#define MARSEN_31 2147483647

class BitsVector {
public:
    explicit BitsVector(uint64_t bits_number_) : bits_number(bits_number_),
                                      bits_array(ceil(static_cast<float>(bits_number_) / 8.0), 0) {}

   ~BitsVector() = default;

    void add(const uint64_t bit_position) {
        bits_array[bit_position / 8] |= (1 << bit_position % 8);
    }

    [[nodiscard]] bool isExist(const uint64_t bit_position) const {
        return ( (bits_array[bit_position / 8] >> bit_position % 8) & 1) != 0;
    }

    [[nodiscard]] uint64_t size() const {
        return bits_number;
    }

private:
    uint64_t bits_number;
    std::vector<uint8_t> bits_array;
};

class FilterBloom {
public:
    FilterBloom(uint64_t n, float P) :
            bits(std::round(-(n * std::log2(P) / std::log(2)))),
            hash_func_number(std::round(-std::log2(P)))
            {
                InitializeFirstPrimes();
            }

    ~FilterBloom() = default;

    void add(const uint64_t& key) {
        for (size_t i = 0; i < hash_func_number; ++i) {
            bits.add(
                    ( (key % MARSEN_31) * (i + 1) % MARSEN_31  + primes[i] % MARSEN_31) % MARSEN_31 % bits.size()
            );
        }
    }

    [[nodiscard]] bool search(const uint64_t& key) const {
        for (size_t i = 0; i < hash_func_number; ++i) {
            if(!bits.isExist(
                    ( (key % MARSEN_31) * (i + 1) % MARSEN_31  + primes[i] % MARSEN_31) % MARSEN_31 % bits.size()
            )) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] std::pair<uint64_t, uint64_t> GetInitializationParameters() const {
        return std::make_pair(bits.size(), hash_func_number);
    }

    void print(std::ostream& os = std::cout) const {
        uint64_t bit_position = 0;
        while (bit_position < bits.size()) {
            os << bits.isExist(bit_position);
            ++bit_position;
        }
        os << std::endl;
    }

private:
    // Ищем до корня числа O(n*sqrt(n))
    void InitializeFirstPrimes() {
        uint64_t i = 1;
        while (primes.size() <= hash_func_number) {
            ++i;
            bool is_prime = true;
            for (uint64_t j = 2; j * j - 1 <= i; ++j) {
                if (i % j == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                primes.push_back(i);
        }
    }

    BitsVector bits;
    uint64_t hash_func_number;
    std::vector<uint64_t> primes;
};

bool isCorrect(const std::string& num) {
    return std::all_of(num.cbegin(), num.cend(),
                       [](char ch) { return std::isdigit(ch); } );
}

int main() {
    std::ios::sync_with_stdio(false);
    std::string command;
    uint64_t n = 0;
    float P = 0.;
    while (std::cin >> command) {
        if (command.empty()) {
            continue;
        } else if (command == "set") {
            std::cin >> n >> P;
            if (n != 0 && P > 0.0 && P < 1.0 &&
            static_cast<uint64_t>(std::round(-1. * std::log2(P))) != 0) {
                break;
            } else {
                std::cout << "error\n";
            }
        } else {
            std::string number;
            if (command != "print") {
                std::cin >> number;
            }
            std::cout << "error\n";
        }
    }

    if (n != 0 && P != 0. && P < 1.) {
        FilterBloom bloom(n, P);
        auto init_param = bloom.GetInitializationParameters();
        std::cout << init_param.first << " " << init_param.second << "\n";

        std::string number;
        while (std::cin >> command) {
            if (command.empty()) {
                continue;
            } else if (command == "print") {
                bloom.print();
            } else {
                std::cin >> number;
                if (command == "add" && isCorrect(number)) {
                    bloom.add(std::stoull(number));
                } else if (command == "search" && isCorrect(number)) {
                    std::cout << bloom.search(std::stoull(number)) << "\n";
                } else if (command == "set") {
                    std::cin >> number;
                    std::cout << "error\n";
                } else {
                    std::cout << "error\n";
                }
            }
        }
    }
    return 0;
}
