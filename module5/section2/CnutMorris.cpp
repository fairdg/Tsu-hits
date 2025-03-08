#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Функция вычисления префикс-функции
vector<size_t> prefix_function(const string& s) {
    size_t n = s.length();
    vector<size_t> pi(n);
    for (size_t i = 1; i < n; ++i) {
        size_t j = pi[i - 1];
        while ((j > 0) && (s[i] != s[j])) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) 
            ++j;
        pi[i] = j;
    }
    return pi;
}

// Функция поиска подстроки с использованием алгоритма КМП
vector<size_t> kmp_search(const string& text, const string& pattern) {
    vector<size_t> pi = prefix_function(pattern);
    vector<size_t> occurrences;
    size_t j = 0; // Индекс для pattern
    for (size_t i = 0; i < text.length(); ++i) {
        while ((j > 0) && (text[i] != pattern[j])) {
            j = pi[j - 1];
        }
        if (text[i] == pattern[j]) {
            ++j;
        }
        if (j == pattern.length()) {
            occurrences.push_back(i - j + 1);
            j = pi[j - 1];
        }
    }
    return occurrences;
}

int main() {
    string s;
    int m;
    cin >> s;
    cin >> m;
    vector<string> patterns(m);
    for (int i = 0; i < m; ++i) {
        cin >> patterns[i];
    }

    for (const string& pattern : patterns) {
        vector<size_t> indices = kmp_search(s, pattern);
        for (size_t index : indices) {
            cout << index << " ";
        }
        cout << endl;
    }

    return 0;
}