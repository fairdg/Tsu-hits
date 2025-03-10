#include <iostream>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

// Функция для нормализации строки
string normalizeString(const string& str) {
    string normalized;
    for (char ch : str) {
        if (isalnum(ch) || ch == ' ') {
            normalized += tolower(ch);
        }
    }
    return normalized;
}

// Функция для удаления слов длиной <= 3 и лишних пробелов
string filterShortWords(const string& str) {
    string filtered;
    string word;
    for (char ch : str) {
        if (ch != ' ') {
            word += ch;
        } else {
            if (word.length() > 3) {
                filtered += word + ' ';
            }
            word.clear();
        }
    }
    if (word.length() > 3) {
        filtered += word + ' ';
    }
    if (!filtered.empty() && filtered.back() == ' ') {
        filtered.pop_back();
    }
    return filtered;
}

// Функция для подсчета общих символов
int countCommonCharacters(const string& s1, const string& s2) {
    unordered_multiset<char> set1(s1.begin(), s1.end());
    unordered_multiset<char> set2(s2.begin(), s2.end());

    int common = 0;
    for (const auto& ch : set1) {
        auto it = set2.find(ch);
        if (it != set2.end()) {
            common++;
            set2.erase(it); // Удаляем символ из второго множества, чтобы избежать повторного подсчета
        }
    }

    return common;
}

int main() {
    string s1, s2;
    getline(cin, s1); // Ввод первой строки
    getline(cin, s2); // Ввод второй строки

    // Нормализация строк
    s1 = normalizeString(s1);
    s2 = normalizeString(s2);

    // Удаление коротких слов и лишних пробелов
    s1 = filterShortWords(s1);
    s2 = filterShortWords(s2);

    // Подсчет общих символов
    int c = countCommonCharacters(s1, s2);

    // Вычисление коэффициента Жаккара
    double jaccard = static_cast<double>(c) / (s1.length() + s2.length() - c);

    // Вывод результата с точностью не менее 10^-6
    cout << fixed << setprecision(10) << jaccard << endl;

    return 0;
}