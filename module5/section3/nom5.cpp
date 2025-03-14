#include <iostream>
#include <set>
#include <string>
#include <cctype>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

string normalize(const string& str) {
    string result;
    for (char c : str) {
        if (isalnum(c) || c == ' ') {
            result += tolower(c);
        }
    }
    
    stringstream removetext(result);
    string word, normalized;
    while (removetext >> word) {
        if (word.length() > 3) {
            if (!normalized.empty()) {
                normalized += " ";
            }
            normalized += word;
        }
    }
    
    return normalized;
}

double jaccard_index(string a, string b) {
    double n = min(a.size(), b.size());
    double m = max(a.size(), b.size());
    double c = 0;
    for (int i = 0; i < n; ++i) if (a[i] == b[i]) c++;
    double result = c / ( n + m - c);
    return result;

}

vector<string> split(const string &s, char delim) {
    vector<string> tokens;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            tokens.push_back(item);
        }
    }
    return tokens;
}

double petyawork(string a, string b) {
    vector<string> a_words = split(a, ' ');
    vector<string> b_words = split(b, ' ');
    double word_count = 0;
    for (const auto& word_a : a_words) {
        for (const auto& word_b : b_words) {
            if (jaccard_index(word_a, word_b) > 0.45) {
                word_count += 1;
            }
        }
    }
    double n = a_words.size();
    double m = b_words.size();
    return word_count / (n + m - word_count);
}

int main() {
    string str;
    vector<string> strings;
    while (getline(cin, str)) {
        strings.push_back(normalize(str));
    }

    for (size_t i = 0; i < strings.size() - 1; ++i) {
        for (size_t j = i + 1; j < strings.size(); ++j) {
            if (petyawork(strings[i], strings[j]) > 0.25) {
                cout << i + 1 << " " << j + 1 << endl;
            }
        }
    }
}