#include <iostream>
#include <string>
#include <string_view>

template <class T>
class Stack {
public:
    Stack(const int capacity) : capacity_(capacity), size_(0),
                                data(new T[capacity]) {}

    ~Stack() { delete[] data; }

    void push(const T& val);
    void pop();
    void print() noexcept;

private:
    int size_;
    int capacity_;

    T* data;
};

template <class T>
void Stack<T>::push(const T& val) {
    if (size_ >= capacity_) {
        std::cout << "overflow" << std::endl;
        return;
    }
    data[size_] = val;
    ++size_;
}

template <class T>
void Stack<T>::pop() {
   if (size_ == 0) {
       std::cout << "underflow" << std::endl;
       return;
   }
   std::cout << data[--size_] << std::endl;
}

template <class T>
void Stack<T>::print() noexcept {
    if (size_ == 0) {
        std::cout << "empty" << std::endl;
        return;
    }
    int i = 0;
    while (i < size_ - 1) {
        std::cout << data[i] << " ";
        ++i;
    }
    std::cout << data[i] << std::endl;
}

bool start_with(const std::string& original, const std::string& check) {
    if (original.rfind(check, 0) == 0) { // pos=0 limits the search to the prefix
        return true;
    }
    return false;
}

std::string get_digit(std::string_view command, std::string_view delta) {
    command.remove_prefix(delta.size());
    return static_cast<std::string>(command);
}


int main() {
    std::string command;
    int n = 0;
    while (std::getline(std::cin, command)) {
        if ( !command.empty() ) {
            if ( start_with(command, "set_size ") ) {
                n = std::stoi(get_digit(command, "set_size "));
                break;
            } else {
                std::cout << "error" << std::endl;
            }
        }
    }
    if (n == 0) {
        while (std::getline(std::cin, command)) {
                std::cout << "error" << std::endl;
        }
    } else {
        Stack<std::string> stack(n);
        while (std::getline(std::cin, command)) {
           if (command.empty()) {}
           else if (start_with(command, "pop") && command.size() == 3) {
               stack.pop();
           } else if (start_with(command, "print") && command.size() == 5) {
               stack.print();
           } else if (start_with(command, "push ")) {
               std::string for_check = get_digit(command, "push ");
               if (for_check.find(' ') == std::string::npos) {
                   stack.push(for_check);
               } else { std::cout << "error" << std::endl; }
           }
           else { std::cout << "error" << std::endl; }
        }
    }
}
