#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <functional>

struct TrieNode {
    explicit TrieNode(std::wstring  key_, bool end = false) : key(std::move(key_)), brother(nullptr),
                                           child(nullptr), is_end(end) {}
    ~TrieNode() = default;

    std::wstring key;
    TrieNode* brother;
    TrieNode* child;
    bool is_end;
};

class TrieTree {
public:
    TrieTree() : root(nullptr) {}

    ~TrieTree() {
        if (root) {
            del_tree(root);
        }
    }

    // Используем для инкапусуляции
    void insert(std::wstring& word);
    // Заполняем первую строку расстояний (для пустой строки),
    // и вызываем рекурсивный поиск для всех братьев корня дерева
    std::set<std::wstring> search(std::wstring& word, size_t maxCost = 1);
private:
    // P.S. Запись из Вики : Временная сложность операций поиска, добавления и удаления
    // элемента из базисного дерева оценивается как O(k), где k — длина обрабатываемого элемента.
    // P.S.S. Но в моей реализации (через односвязный список) я могу предположить худший случай,
    // который зависит от количсетва узлов. Допустим я ввожу слово {$str}, в худшем случае
    // я пройду по всем братьям корня, потом в последнем брате, перейду и пройду по всем его детям и
    // потом у последнего ребёнка так же по всем его братьям и т.д. Таким образом я получаю сложность O(n*m)
    // где n - длина слова, m - количество узлов в дереве.
    // P.S.S.S. В реализации не сжатого дерева проблема решалась просто, использовал Хеш-таблицу
    // и поэтому не приходилось проходиться по всем братьям и детям)
    TrieNode* insert(TrieNode* node, const std::wstring& word, size_t count = 0);

    // Сложность O(n*m), где n - длина слова, m - количество узлов в дереве
    // От корня, по его братьям и вниз по их детям проходим рекурсивно проверяя узлы на количество ошибок.
    // Если ошибок становится больше 1, то дальше по этому узлу и его детям мы не идём
    // P.S. Пользоваля данной статьёй : http://stevehanov.ca/blog/?id=114
    void searchRecursive(const TrieNode* node, const std::wstring& letters,
                         const std::wstring& word, std::vector<size_t> prev_prev_row, std::vector<size_t> previous_row,
                         std::set<std::wstring>& result, std::wstring current_word,
                         size_t maxCost);

    // Вычисляет длину наибольшего общего префикса двух строк
    static size_t prefix(const std::wstring& insert_str, const std::wstring& key);

    // Если длина общего префикса текущего ключа и текущей строки x больше нуля,
    // но меньше длины ключа (второй случай недачного поиска),
    // то надо разбить текущий узел на два, оставив в родительском узле
    // найденный префикс, и поместив в дочерний узел p оставшуюся часть ключа
    static void split(TrieNode* node, size_t k);

    // Перевод корректки из верхней в нижнюю
    static std::wstring to_lowercase(std::wstring& str);

    // Рекурсивная функция деструктора
    void del_tree(TrieNode* node);

    TrieNode* root;
};

// public
void TrieTree::insert(std::wstring& word) {
    to_lowercase(word);
    root = insert(root, word);
}

std::set<std::wstring> TrieTree::search(std::wstring& word, size_t maxCost) {
    to_lowercase(word);
    // build first row
    std::vector<size_t> current_row;
    current_row.reserve(word.size() + 2);
    for (size_t i = 0; i <= word.size() + 1; ++i) {
        current_row.push_back(i);
    }
    // result
    std::set<std::wstring> result;

    std::wstring current_word;
    std::vector<size_t> prev_row;
    TrieNode* node = root;
    // recursively search each branch of the trie
    while (node) {
        searchRecursive(node, node->key, word, prev_row, current_row,
                        result, current_word, maxCost);
        node = node->brother;
    }
    return result;
}

// private

TrieNode* TrieTree::insert(TrieNode* node, const std::wstring& word, size_t count) {
    if (!count) { count = word.size(); }
    if (!node) {
        return new TrieNode(word, true);
    }
    size_t k = prefix(word, node->key);
    if (k == 0) {
        node->brother = insert(node->brother, word, count);
    } else if (k < count) {
        if (k < node->key.size()) {
            split(node, k);
            node->is_end = false;
        }
        node->child = insert(node->child, word.substr(k, word.size()), count - k);
    } else if (k == count) {
        if (k < node->key.size()) {
            split(node, k);
        }
        node->is_end = true;
    }
    return node;
}

void TrieTree::searchRecursive(const TrieNode* node, const std::wstring& letters,
                     const std::wstring& word, std::vector<size_t> prev_prev_row, std::vector<size_t> previous_row,
                     std::set<std::wstring>& result, std::wstring current_word,
                     size_t maxCost) {
    size_t columns = word.size() + 2;
    std::vector<size_t> current_row;
    for (size_t i = 0; i < letters.size(); ++i) {
        current_word.push_back(letters[i]);

        current_row.clear();
        current_row.reserve(word.size() + 2);
        current_row.push_back( previous_row[0] + 1);

        for (size_t column = 1; column < columns; ++column) {
            size_t insert_cost = current_row[column - 1] + 1;
            size_t delete_cost = previous_row[column] + 1;

            size_t replace_cost;
            if (word[column - 1] != letters[i]) {
                replace_cost = previous_row[ column - 1 ] + 1;
            } else {
                replace_cost = previous_row[ column - 1 ];
            }

            current_row.push_back(std::min(std::min( insert_cost, delete_cost), replace_cost));
            if (!prev_prev_row.empty() && column >= 2 && word[column - 2] == letters[i] && word[column - 1] == current_word[current_word.size() - 2]) {
                current_row[column] = std::min(current_row[column], prev_prev_row[column - 2] + 1);
            }
        }

        if (current_row[current_row.size() - 2] <= maxCost && node->is_end && i == letters.size() - 1) {
            result.insert(current_word);
        }

        if (*std::min_element(current_row.begin(), current_row.end()) <= maxCost && i != letters.size() - 1) {
            prev_prev_row = previous_row;
            previous_row = current_row;
        } else { break; }
    }

    if (*std::min_element(current_row.begin(), current_row.end()) <= maxCost) {
        TrieNode* child_node = node->child;
        while (child_node) {
            searchRecursive(child_node, child_node->key, word, previous_row, current_row,
                            result, current_word, maxCost);
            child_node = child_node->brother;
        }
    }
}

size_t TrieTree::prefix(const std::wstring& insert_str, const std::wstring& key) {
    for (size_t k = 0; k < insert_str.size(); ++k) {
        if (k == key.size() || insert_str[k] != key[k])
            return k;
    }
    return insert_str.size();
}

void TrieTree::split(TrieNode* node, size_t k) {
    TrieNode* p = new TrieNode(node->key.substr(k, node->key.size()), node->is_end);
    p->child = node->child;
    node->child = p;
    node->key = node->key.substr(0, k);
}

std::wstring TrieTree::to_lowercase(std::wstring& str) {
    std::locale locale_{""};
    std::transform(str.begin(), str.end(), str.begin(),
                   std::bind(std::tolower<wchar_t>, std::placeholders::_1, std::cref(locale_)));
    return str;
}

void TrieTree::del_tree(TrieNode* node) {
    if (node->brother) {
        del_tree(node->brother);
    }
    if (node->child) {
        del_tree(node->child);
    }
    delete node;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::locale locale_{""};
    std::wcin.imbue(locale_);
    std::wcout.imbue(locale_);
    TrieTree tree;
    std::wstring str;

    // filling correct words in tree
    int words_count = 0;
    std::wcin >> words_count;

    std::wcin.ignore();

    while (words_count) {
        std::getline(std::wcin, str);
        tree.insert(str);
        --words_count;
    }

    // Return result
    while (std::getline(std::wcin,str)) {
        if (!str.empty()) {
            std::wcout << str;
            std::set<std::wstring> str_to_print = tree.search(str);
            if (str_to_print.empty()) {
                std::wcout << L" -?\n";
            } else {
                if (str_to_print.find(str) != str_to_print.end()) {
                    std::wcout << L" - ok\n";
                } else {
                    std::wstring correct_words = L" ->";
                    for (const auto& print : str_to_print) {
                        correct_words += L" " + print + L",";
                    }
                    correct_words.pop_back();
                    std::wcout << correct_words << L"\n";
                }
            }
        }
    }

    return 0;
}
