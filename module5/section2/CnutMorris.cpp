#include <iostream>
#include <vector>
#include <string>

using namespace std;

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


vector<size_t> search(const string& text, const string& reg) {
    vector<size_t> pi = prefix_function(reg);
    vector<size_t> vec;
    size_t j = 0;
    for (size_t i = 0; i < text.length(); ++i) {
        while ((j > 0) && (text[i] != reg[j])) {
            j = pi[j - 1];
        }
        if (text[i] == reg[j]) {
            ++j;
        }
        if (j == reg.length()) {
            vec.push_back(i - j + 1);
            j = pi[j - 1];
        }
    }
    return vec;
}

int main() {
    string s;
    int m;
    cin >> s;
    cin >> m;
    vector<string> regs(m);
    for (int i = 0; i < m; ++i) {
        cin >> regs[i];
    }

    for (const string& reg : regs) {
        vector<size_t> indexs = search(s, reg);
        for (size_t index : indexs) {
            cout << index + 1 << " "; 
        }
        cout << endl;
    }

    return 0;
}