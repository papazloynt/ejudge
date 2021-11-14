#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <set>

// Сделать Wchar
// Изменить дерево на сжато
// Изменить стиль кода на красивый

struct TrieNode {
    TrieNode() : is_end(false), children() {}

    bool is_end;
    std::unordered_map<char, TrieNode*> children;
};

class TrieTree {
public:
    TrieTree() : root(new TrieNode()) {}
    ~TrieTree() {
        if (root) {
            del_tree(root);
        }
    }

    std::set<std::string> search(const std::string& word, size_t maxCost = 1) {
        // build first row
        std::vector<size_t> current_row;
        current_row.reserve(word.size() + 2);
        for (size_t i = 0; i <= word.size(); ++i) {
            current_row.push_back(i);
        }
        // result
        std::set<std::string> result;

        std::string current_word;
        std::vector<size_t> prev_row;
        // recursively search each branch of the trie
        for (const auto& child : root->children) {
            searchRecursive(child.second, child.first, word, prev_row, current_row,
                            result, current_word, maxCost);
        }
        return result;
    }

    void searchRecursive(TrieNode* node, const char letter,
                         const std::string& word, std::vector<size_t>& prev_prev_row, std::vector<size_t>& previous_row,
                         std::set<std::string>& result, std::string current_word,
                         size_t maxCost) {
        size_t columns = word.size() + 2;
        current_word.push_back(letter);
        std::vector<size_t> current_row;
        current_row.reserve(word.size() + 2);
        current_row.push_back( previous_row[0] + 1);

        for (size_t column = 1; column < columns; ++column) {
            size_t insert_cost = current_row[column - 1] + 1;
            size_t delete_cost = previous_row[column] + 1;

            size_t replace_cost;
            if (word[column - 1] != letter) {
                replace_cost = previous_row[ column - 1 ] + 1;
            } else {
                replace_cost = previous_row[ column - 1 ];
            }

            current_row.push_back(std::min(std::min( insert_cost, delete_cost), replace_cost));
            if (!prev_prev_row.empty() && column >= 2 && word[column - 2] == letter && word[column - 1] == current_word[current_word.size() - 2]) {
                current_row[column] = std::min(current_row[column], prev_prev_row[column - 2] + 1);
            }
        }

        if (current_row[current_row.size() - 2] <= maxCost && node->is_end) {
            result.insert(current_word);
        }

        if (*std::min_element(current_row.begin(), current_row.end()) <= maxCost) {
            for (const auto& child : node->children) {
                searchRecursive(child.second, child.first, word, previous_row, current_row,
                                result, current_word, maxCost);
            }
        }
    }

    void insert(const std::string& str) {
        TrieNode* ptr = root;
        for (const char ch : str) {
            auto iter = ptr->children.find(ch);
            if (iter != ptr->children.end()) {
                ptr = iter->second;
            } else {
                ptr->children[ch] = new TrieNode();
                ptr = ptr->children.at(ch);
            }
        }
        if (ptr) {
            ptr->is_end = true;
        }
    }

private:

    void del_tree(TrieNode* node) {
        for (auto& i : node->children) {
            del_tree(i.second);
        }
        delete node;
    }

    TrieNode* root;
};














int main() {
    std::ios_base::sync_with_stdio(false);
    std::locale loc{""};
    std::wcin.imbue(loc);
    std::wcout.imbue(loc);
    TrieTree dictionary;
    std::string string_;
    {
        int word_count = 0;
        std::wcin >> word_count;
        std::wcin.ignore();
        while (word_count != 0) {
            std::getline(std::cin, string_);
            dictionary.insert(string_);
            --word_count;
        }
    }
    while (std::getline(std::cin,string_)) {
        if (!string_.empty()) {
            std::cout << string_;
            std::set<std::string> print_ = dictionary.search(string_);
            if (print_.empty()) {
                std::cout << " -?\n";
            } else {
                if (print_.find(string_) != print_.end())
                    std::cout << " - ok\n";
                else {
                    std::string str = " ->";
                    for (const auto& print : print_) {
                        str += " " + print + ",";
                    }
                    str.pop_back();
                    std::cout << str<<"\n";
                }
            }
        }
    }
    return 0;
}
