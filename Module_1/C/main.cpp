#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

template <class T>
class Queue {
public:
    Queue(int capacity) : capacity_(capacity), size_(0),
                          data(new T[capacity])
                          {
                            head = tail = data;
                          }

    ~Queue() {
        delete[] data;
    }

    void push(const T& val, std::ofstream& out);
    void pop(std::ofstream& out);
    void print(std::ofstream& out) noexcept;

private:
    int size_;
    int capacity_;

    T* head;
    T* tail;
    T* data;
};

template <class T>
void Queue<T>::push(const T& val, std::ofstream& out) {
    if (size_ >= capacity_) {
        out << "overflow" << std::endl;
        return;
    }
    *tail = val;
    ++tail;
    ++size_;
    // Check overflow
    if (tail >= data + capacity_) {
        tail = data;
    }
}

template <class T>
void Queue<T>::pop(std::ofstream& out) {
    if (size_ == 0) {
        out << "underflow" << std::endl;
        return;
    }
    out << *head << std::endl;
    ++head;
    --size_;
    // Check overflow
    if (head >= data + capacity_) {
        head = data;
    }
}

template <class T>
void Queue<T>::print(std::ofstream& out) noexcept {
    if (size_ == 0) {
        out << "empty" << std::endl;
        return;
    }
    T* tmp = head;
    T* last = tail == data ? data + capacity_ - 1 : tail - 1;
    while (tmp != last) {
        out << *tmp << " ";
        ++tmp;
        if (tmp >= data + capacity_) {
            tmp = data;
        }
    }
    out << *last << std::endl;
}

bool start_with(const std::string& original, const std::string& check) {
    if (original.rfind(check, 0) == 0) {
        return true;
    }
    return false;
}

std::string get_digit(std::string_view command, std::string_view delta) {
    command.remove_prefix(delta.size());
    return static_cast<std::string>(command);
}

int main(int argc, char** argv) {
    std::string command;
    int n = 0;

    std::ifstream in(argv[1], std::ios::in);
    std::ofstream out;
    out.open(argv[2]);

    if (in.is_open() && out.is_open()) {
        while (std::getline(in, command)) {
            if (!command.empty()) {
                if (start_with(command, "set_size ")) {
                    n = std::stoi(get_digit(command, "set_size "));
                    break;
                } else {
                    out << "error" << std::endl;
                }
            }
        }

        Queue<std::string> queue(n);
        while (std::getline(in, command)) {
            if (command.empty()) {}
            else if (start_with(command, "pop") && command.size() == 3) {
                queue.pop(out);
            } else if (start_with(command, "print") && command.size() == 5) {
                queue.print(out);
            } else if (start_with(command, "push ")) {
                std::string for_check = get_digit(command, "push ");
                if (for_check.find(' ') == std::string::npos) {
                    queue.push(for_check, out);
                } else { out << "error" << std::endl; }
            } else { out << "error" << std::endl; }
        }
    }
}
