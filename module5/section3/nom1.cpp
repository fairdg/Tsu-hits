#include <iostream>
#include <string>
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

int main() {
    string line;
    while (getline(cin, line)) { 
        string normalized = normalizeText(line);
        cout << normalized << endl;
    }
    return 0;
}