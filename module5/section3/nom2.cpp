#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;

string normalizeText(const string& s) {
    string result;
    for (char ch : s) {
        if (isalnum(ch) || isspace(ch)) {
            result += tolower(ch);
        }
    }
    return result;
}

string removeText(const string& s) {
    stringstream ss(s);
    string word, result;
    while (ss >> word) {
        if (word.length() > 3) {
            if (!result.empty()) {
                result += ' ';
            }
            result += word;
        }
    }
    return result;
}

int main() {
    string line;
    while (getline(cin, line)) {
        string normalized = normalizeText(line);
        string finalText = removeText(normalized);
        cout << finalText ;
    }
}