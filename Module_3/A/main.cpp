#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <tuple>

using Properties = std::pair<uint64_t, uint64_t>;

class Knapsack {
public:
    Knapsack(uint64_t max_mas_, std::vector<Properties> items_) :
                        max_mas(max_mas_), items(std::move(items_)) {}
    Knapsack& operator=(Knapsack&) = delete;
    ~Knapsack() = default;

    std::tuple<uint64_t, uint64_t, std::vector<uint64_t>> MaxCount() {
        // Чтобы не использовать много памяти, берём НОД от всех весов
        uint64_t nod = NOD(max_mas, NODMultiple(items));
        // Храним ценность и предметы, которые мы взяли
        std::vector<std::pair<uint64_t, uint64_t>> prev_raw(max_mas / nod + 1, std::make_pair(0,0));
        // По всем предметам
        for (uint64_t k = 0; k < items.size(); ++k) {
            std::vector<std::pair<uint64_t, uint64_t>> current_raw(max_mas / nod + 1, std::make_pair(0,0));
            for (uint64_t s = 0; s <= max_mas / nod; ++s) {
                if (s >= items[k].first / nod) {
                    current_raw[s] = std::max(prev_raw[s],
                                              std::make_pair(prev_raw[s - items[k].first / nod].first + items[k].second,
                                                              prev_raw[s - items[k].first / nod].second | (1 << k)),
                                              [] (std::pair<uint64_t, uint64_t> l,
                                                        std::pair<uint64_t, uint64_t> r)
                                                        { return l.first <= r.first;} );

                } else {
                    current_raw[s] = prev_raw[s];
                }
            }
            prev_raw = std::move(current_raw);
        }
        auto tmp = GetIndexesAndWeight(prev_raw[prev_raw.size() - 1].second);
        return std::make_tuple(tmp.first, prev_raw[prev_raw.size() - 1].first, tmp.second);
    }

private:
    [[nodiscard]] uint64_t NOD(uint64_t a, uint64_t b) const {
        while (a && b)
            b < a ? a %= b : b %= a;
        return a | b;
    }

    [[nodiscard]] uint64_t NODMultiple(const std::vector<Properties>& items_) const {
        uint64_t n = 0;
        for (const auto& item : items_) {

            n = NOD(n, item.first);
        }
        return n;
    }

    [[nodiscard]] std::pair<uint64_t, std::vector<uint64_t>> GetIndexesAndWeight(uint64_t elements) const {
        int counter = 0;
        uint64_t weight = 0;
        std::vector<uint64_t> elements_;
        while (elements > 0) {
            if (elements & 1) {
                elements_.push_back(counter + 1);
                weight += items[counter].first;
            }
            elements = elements / 2;
            ++counter;
        }
        return std::make_pair(weight, elements_);
    }

    uint64_t max_mas;
    std::vector<Properties> items;
};

bool IsDigit(const std::string& d) {
    return std::all_of(d.cbegin(), d.cend(),
                       [](char ch) { return std::isdigit(ch); } );
}

std::pair<uint64_t, uint64_t> ParseTwoDigits(const std::string& line) {
    auto pos = std::find(line.cbegin(), line.cend(), ' ');
    if (pos != line.cend()) {
        if (IsDigit( { line.cbegin(), pos }) &&
            IsDigit( { pos + 1, line.cend()}))  {
            return std::make_pair(std::stoull(std::string{ line.cbegin(), pos }),
                                  std::stoull(std::string{ pos + 1, line.cend()}));
        }
    }
    throw std::invalid_argument("error");
}

int main() {
    uint64_t max_mas;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        } else if (IsDigit(line)) {
            max_mas = std::stoull(line);
            break;
        } else {
            std::cout << "error" << std::endl;
        }
    }

    std::vector<Properties> elements;
    std::string weight, cost;
    while (std::getline(std::cin, line)) {
        try {
            auto digits = ParseTwoDigits(line);
            elements.emplace_back(digits.first, digits.second);
        } catch (...) {
            std::cout << "error" << std::endl;
        }
    }

    Knapsack k(max_mas, elements);
    auto res = k.MaxCount();
    std::cout << std::get<0>(res) << " " << std::get<1>(res) << std::endl;
    for (const auto& elem : std::get<2>(res)) {
        std::cout << elem << std::endl;
    }
    return 0;
}
