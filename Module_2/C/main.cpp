#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>

template <typename K, typename V>
struct NodeMinHeap {
    NodeMinHeap(K key_, V value_) : key(key_), value(value_) {}
    NodeMinHeap(const NodeMinHeap& heap) = default;
    ~NodeMinHeap() = default;

    friend bool operator < (const NodeMinHeap& l, const NodeMinHeap& r) {
        return l.key < r.key;
    }

    K key;
    V value;
};

template <typename K, typename V>
class MinHeap {
public:
    MinHeap() = default;
    ~MinHeap() = default;

    void add(const K& key, const V& value) {
        if (tree_indexes.find(key) != tree_indexes.end()) {
            throw std::logic_error("Already was exist");
        }
        tree_indexes[key] =  heap.size();
        heap.push_back({key, value});
        siftUp(heap.size() - 1);
    }

    void set(const K& key, const V& value) {
        std::tuple<bool, size_t, V> elem = search(key);
        if (!std::get<0>(elem)) {
            throw std::logic_error("There isn't element to set");
        }
        heap[std::get<1>(elem)].value = value;
    }

    void erase(const K& key) {
        std::tuple<bool, size_t, V> elem_del = search(key);

        if (!std::get<0>(elem_del)) {
            throw std::logic_error("There isn't elements to delete");
        }

        std::swap(tree_indexes[key], tree_indexes[heap[heap.size() - 1].key]);
        tree_indexes.erase(key);
        heap[std::get<1>(elem_del)] = heap[heap.size() - 1];
        heap.pop_back();

        if (std::get<1>(elem_del) == 0 ||
            key < heap[std::get<1>(elem_del)].key) {
            siftDown(std::get<1>(elem_del));
        } else {
            siftUp(std::get<1>(elem_del));
        }
    }

    std::tuple<bool, size_t, V> search(const K& key) const {
        auto iter = tree_indexes.find(key);
        if (iter == tree_indexes.end()) {
            return std::make_tuple(false, 0,
                                   V());
        }
        return std::make_tuple(true, iter->second,
                               heap[iter->second].value);
    }

    std::tuple<K, size_t, V> max() const {
        if (heap.empty()) {
            throw std::logic_error("Heap is empty");
        }
        NodeMinHeap<K, V> max = heap[0];
        size_t max_index = 0;
        for (size_t i = 0; i < heap.size(); ++i) {
            if (getLeft(i) >= heap.size() &&
                getRight(i) >= heap.size()) {
                if (max < heap[i]) {
                    max = heap[i];
                    max_index = i;
                }
            }
        }
        return std::make_tuple(max.key, max_index, max.value);
    }

    std::tuple<K, size_t, V> min() const {
        if (heap.empty()) {
            throw std::logic_error("Heap is empty!");
        }
        return std::make_tuple(heap[0].key, 0, heap[0].value);
    }

    std::pair<K, V> extract() {
        if (heap.empty()) {
            throw std::logic_error("Heap is empty!");
        }
        std::pair<K, V> p = std::make_pair(heap[0].key, heap[0].value);
        erase(heap[0].key);
        return  p;
    }

    void print(std::ostream& os) {
        size_t current_node_index = 0;
        size_t counter = 0;
        size_t two_degree = 1;
        size_t two_degree_counter = 0;

        while (true) {
            print_node(current_node_index, os);

            if (current_node_index < heap.size()) { ++counter; }

            ++two_degree_counter;
            if (two_degree_counter == two_degree) {
                two_degree_counter = 0;
                two_degree <<= 1;
                os << std::endl;
                if (counter >= heap.size()) { break; }
            } else {
                os << " ";
            }
            ++current_node_index;
        }
    }

private:
    std::vector<NodeMinHeap<K, V>> heap;
    std::unordered_map<K, size_t> tree_indexes;

    [[nodiscard]] size_t getParent(const size_t i) const { return (i - 1) / 2; }
    [[nodiscard]] size_t getLeft(const size_t i) const { return 2 * i + 1; }
    [[nodiscard]] size_t getRight(const size_t i) const { return 2 * i + 2; }

    void siftUp(int i) {
        while (i && heap[i] < heap[getParent(i)]) {
            std::swap(tree_indexes[heap[i].key], tree_indexes[heap[getParent(i)].key]);
            std::swap (heap[i], heap[getParent(i)]);
            i = getParent(i);
        }
    }

    void siftDown(int i) {
        while (getLeft(i) < heap.size()) {
            int left = getLeft(i);
            int right = getRight(i);
            int small = left;

            if (right < heap.size() && heap[right] < heap[left]) {
                small = right;
            }

            if (heap[i] < heap[small]) { break; }

            std::swap(tree_indexes[heap[i].key], tree_indexes[heap[small].key]);
            std::swap (heap[i], heap[small]);
            i = small;
        }
    }

    void print_node(size_t current_node_index, std::ostream& os) {
        if (current_node_index >= heap.size()) {
            os << "_";
        } else if (current_node_index == 0) {
            os << "[" << heap[0].key << " " << heap[0].value << "]";
        } else {
            os << "[" << heap[current_node_index].key << " " << heap[current_node_index].value
            << " " << heap[getParent(current_node_index)].key << "]";
        }
    }
};

int main() {
    MinHeap<int64_t , std::string> heap;
    std::string command;

    while (std::cin >> command) {
        if (command.empty()) { }
        else if (command == "add") {
          try {
              int64_t key;
              std::string value;
              std::cin >> key >> value;
              heap.add(key, value);
          } catch (...) {
              std::cout << "error" << std::endl;
          }

        } else if (command == "set") {
           try {
               int64_t key;
               std::string value;
               std::cin >> key >> value;
               heap.set(key, value);
           } catch (...) {
               std::cout << "error" << std::endl;
           }
        } else if (command == "delete") {
            try {
                int64_t key;
                std::string value;
                std::cin >> key;
                heap.erase(key);
            } catch (...) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "min") {
            try {
                auto min = heap.min();
                std::cout << std::get<0>(min) << " "
                          << std::get<1>(min) << " "
                          << std::get<2>(min) << std::endl;
            } catch (...) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "max") {
            try {
                auto max = heap.max();
                std::cout << std::get<0>(max) << " "
                          << std::get<1>(max) << " "
                          << std::get<2>(max) << std::endl;
            } catch (...) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "extract") {
            try {
                auto min_ex = heap.extract();
                std::cout << min_ex.first << " " << min_ex.second << std::endl;
            } catch (...) {
                std::cout << "error" << std::endl;
            }
        }
        else if (command == "print") {
            heap.print(std::cout);
        }
        else if (command == "search") {
            int64_t key;
            std::cin >> key;
            auto elem = heap.search(key);
            if (std::get<0>(elem)) {
                std::cout << "1 " << std::get<1>(elem) << " "
                          << std::get<2>(elem) << std::endl;
            } else { std::cout << "0" << std::endl; }
        }
        else {
            std::cout << "error" << std::endl;
        }
    }
    return 0;
}