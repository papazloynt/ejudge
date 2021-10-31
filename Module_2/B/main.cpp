#include <iostream>
#include <utility>
#include <queue>
#include <string>
#include <string_view>

#define ROTATION(front, back)                      \
    Node<T>* tmp = elem->front;                    \
    if (tmp) {                                     \
        elem->front = tmp->back;                   \
        if (tmp->back) {                           \
            tmp->back->parent = elem;              \
        }                                          \
        tmp->parent = elem->parent;                \
    }                                              \
    if (!elem->parent) {                           \
        root = tmp;                                \
    } else if (elem == elem->parent->back) {       \
        elem->parent->back = tmp;                  \
    } else {                                       \
        elem->parent->front = tmp;                 \
    }                                              \
    if (tmp) {                                     \
        tmp->back = elem;                          \
    }                                              \
    elem->parent = tmp;

template <typename T>
struct Node {
    Node* left, * right;
    Node* parent;
    int64_t key;
    T data;

    Node(int64_t key_, const T&  data_) : left(nullptr), right(nullptr),
                                           parent(nullptr), key(key_),
                                           data(data_) {}
  /*  ~Node() { }*/
};

template <typename T>
class SplayTree {
public:
    SplayTree() : root(nullptr), size_(0) {}
    ~SplayTree() {
        if (root) {
            del_tree(root);
        }
    }

    void add(int64_t key, T data);
    void erase(int64_t key);

    void set (int64_t key, const T& data);
    std::pair<bool, T> search(int64_t key);

    std::pair<int64_t, T> min();
    std::pair<int64_t, T> max();

    [[nodiscard]] bool empty() const { return root == nullptr; }

    void print();

private:
    void left_rotate(Node<T>* elem) { ROTATION(right, left) }
    void right_rotate(Node<T>* elem) { ROTATION(left, right) }

    void splay(Node<T>* elem);
    // return parent and child
    std::pair<Node<T>*, Node<T>*> find(int64_t key);

    // for erase
    Node<T>* max_without_splay(Node<T>* node);

    void print_node(Node<T>* node);

    void del_tree(Node<T>* node) {
        if (node->left) {
            del_tree(node->left);
        }
        if (node->right) {
            del_tree(node->right);
        }
        delete node;
    }


    Node<T>* root;
    size_t size_;
};

template <class T>
void SplayTree<T>::add(int64_t key, T data) {
    Node<T>* new_node = root;
    Node<T>* parent = nullptr;

    while (new_node) {
        parent = new_node;
        if (new_node->key < key) {
            new_node = new_node->right;
        } else if (new_node->key > key) {
            new_node = new_node->left;
        } else {
            splay(new_node);
            throw std::invalid_argument("error");
        }
    }

    new_node = new Node<T>(key, data);
    new_node->parent = parent;

    if (!parent) {
        root = new_node;
    } else if (parent->key < new_node->key) {
        parent->right = new_node;
    } else {
        parent->left = new_node;
    }

    ++size_;
    splay(new_node);
}

template <typename T>
void SplayTree<T>::erase(int64_t key) {
    if (empty()) {
        throw std::invalid_argument("error");
    }
    std::pair<Node<T>*, Node<T>*> del_node = find(key);
    if (!del_node.second) {
        splay(del_node.first);
        root = del_node.first;
        throw std::invalid_argument("error");
    }

    Node<T>* node = del_node.second;
    splay(node);

    if (node->left != nullptr && node->right != nullptr) {
        splay(max_without_splay(node->left));
        splay(node);
        root = node->left;
        root->right = node->right;
        node->right->parent = root;
        root->parent = nullptr;
    } else if (node->left != nullptr && node->right == nullptr) {
        root = node->left;
        root->parent = nullptr;
    } else if (node->left == nullptr && node->right != nullptr) {
        root = node->right;
        root->parent = nullptr;
    } else {
        root = nullptr;
    }
    delete node;
    --size_;
}

template <class T>
void SplayTree<T>::set(int64_t key, const T& data) {
    if (empty()) { throw std::invalid_argument("error"); }

    std::pair<Node<T>*, Node<T>*> tmp = find(key);
    if (tmp.second) {
        tmp.second->data = data;
        splay(tmp.second);
    } else {
        splay(tmp.first);
        throw std::invalid_argument("error");
    }
}

template <class T>
std::pair<bool, T> SplayTree<T>::search(int64_t key) {
    if (empty()) { return std::make_pair(false, T()); }

    std::pair<Node<T>*, Node<T>*> tmp = find(key);
    if (tmp.second) {
        splay(tmp.second);
        return std::make_pair(true, tmp.second->data);
    }
    splay(tmp.first);
    return std::make_pair(false, T());
}

template <class T>
std::pair<int64_t, T> SplayTree<T>::min() {
    if (empty()) { throw std::invalid_argument("error"); }

    Node<T>* ptr = root;
    while (ptr->left) {
        ptr = ptr->left;
    }
    splay(ptr);

    return std::make_pair(ptr->key, ptr->data);
}

template <class T>
std::pair<int64_t, T> SplayTree<T>::max() {
    if (empty()) { throw std::invalid_argument("error"); }

    Node<T>* ptr = root;
    while (ptr->right) {
        ptr = ptr->right;
    }
    splay(ptr);

    return std::make_pair(ptr->key, ptr->data);
}

template <class T>
void SplayTree<T>::print() {
    std::queue<Node<T>*> q;
    q.push(root);

    size_t counter = 0;
    size_t two_degree = 1;
    size_t two_degree_counter = 0;

    while (true) {
        Node<T>* node = q.front();
        q.pop();
        print_node(node);

        if (node) { ++counter; }

        if (node) {
            q.push(node->left);
            q.push(node->right);
        } else {
            q.push(nullptr);
            q.push(nullptr);
        }
        ++two_degree_counter;
        if (two_degree_counter == two_degree) {
            two_degree_counter = 0;
            two_degree *= 2;
            std::cout << std::endl;
            if (counter >= size_) { break; }
        } else {
            std::cout << " ";
        }
    }
}

template <class T>
void SplayTree<T>::splay(Node<T>* elem) {
    while (elem->parent) {
        // Parent is root
        if (!elem->parent->parent) {
            if (elem->parent->left == elem) { // ZIG
                right_rotate(elem->parent);
            } else { // ZAG
                left_rotate(elem->parent);
            }
        } else if (elem->parent->left == elem && elem->parent->parent->left == elem->parent) { // ZIG-ZIG
            right_rotate(elem->parent->parent);
            right_rotate(elem->parent);
        } else if (elem->parent->right == elem && elem->parent->parent->right == elem->parent) { // ZAG-ZAG
            left_rotate(elem->parent->parent);
            left_rotate(elem->parent);
        } else if (elem->parent->left == elem && elem->parent->parent->right == elem->parent) { // ZIG-ZAG
            right_rotate(elem->parent);
            left_rotate(elem->parent);
        } else {                                                                                // ZAG-ZIG
            left_rotate(elem->parent);
            right_rotate(elem->parent);
        }
    }
}

template <class T>
std::pair<Node<T>*, Node<T>*> SplayTree<T>::find(const int64_t key) {
    Node<T>* ptr_parent = root;
    Node<T>* ptr_child = root;

    while (ptr_child) {
        if (ptr_child->key < key) {
            ptr_parent = ptr_child;
            ptr_child = ptr_child->right;
        } else if (key < ptr_child->key) {
            ptr_parent = ptr_child;
            ptr_child = ptr_child->left;
        } else {
            return std::make_pair(ptr_parent, ptr_child);
        }
    }
    return std::make_pair(ptr_parent, nullptr);
}

template <typename T>
Node<T>* SplayTree<T>::max_without_splay(Node<T>* node) {
    while (node->right) { node = node->right;}
    return node;
}

template <typename T>
void SplayTree<T>::print_node(Node<T>* node) {
   if (!node) {
       std::cout << "_";
   } else if (!node->parent) {
       std::cout << "[" << node->key << " " << node->data << "]";
   } else {
       std::cout << "[" << node->key << " " << node->data << " " << node->parent->key << "]";
   }
}

bool start_with(const std::string& original, const std::string& check) {
    if (original.rfind(check, 0) == 0) { // pos=0 limits the search to the prefix
        return true;
    }
    return false;
}

std::string delete_start(std::string_view command, std::string_view delta) {
    command.remove_prefix(delta.size());
    return static_cast<std::string>(command);
}

std::pair<int64_t, std::string> key_value(std::string key_and_value) {
    std::string key;
    size_t i = 0;
    while (key_and_value[i] != ' ') {
        key += key_and_value[i];
        ++i;
    }
    key += " ";
    std::string value = delete_start(key_and_value, key);
    return std::make_pair(std::stoll(key), value);
}

int main() {
    std::string command;
    SplayTree<std::string> splay_tree;
    while (std::getline(std::cin, command)) {
        try {
            if (command.empty()) {}
            else if (start_with(command, "add ")) {
                std::pair<int64_t, std::string> p = key_value(delete_start(command, "add "));
                splay_tree.add(p.first, p.second);
            } else if (start_with(command, "set ")) {
                std::pair<int64_t, std::string> p = key_value(delete_start(command, "set "));
                splay_tree.set(p.first, p.second);
            } else if (start_with(command, "delete ")) {
                std::string key = delete_start(command, "delete ");
                splay_tree.erase(std::stoll(key));
            } else if (start_with(command, "search ")) {
                std::string key = delete_start(command, "search ");
                auto p = splay_tree.search(std::stoll(key));
                if (p.first) {
                    std::cout << 1 << " " << p.second << std::endl;
                } else {
                    std::cout << 0 << std::endl;
                }
            } else if (command == "min") {
                auto p = splay_tree.min();
                std::cout << p.first << " " << p.second << std::endl;
            } else if (command == "max") {
                auto p = splay_tree.max();
                std::cout << p.first << " " << p.second << std::endl;
            } else if (command == "print") {
                splay_tree.print();
            } else {
                std::cout << "error" << std::endl;
            }
        } catch (...) {
            std::cout << "error" << std::endl;
        }
    }
    return 0;
}