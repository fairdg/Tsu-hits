#include <iostream>
#include <set>
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>

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

int countMatches(const string& s1, const string& s2) {
    int matches = 0;
    int length = min(s1.length(), s2.length());
    for (int i = 0; i < length; ++i) {
        if (s1[i] == s2[i]) {
            ++matches;
        }
    }
    return matches;
}

double jaccard_index(string a, string b) {
    double n = min(a.size(), b.size());
    double m = max(a.size(), b.size());
    double c = 0;
    for (int i = 0; i < n; ++i) if (a[i] == b[i]) c++;
    double result = c / ( n + m - c);
    return result;

}

int main() {
    string line1, line2;
    getline(cin, line1);
    getline(cin, line2);
    
    string normalized1 = normalize(line1);
    string normalized2 = normalize(line2);
    string finalText1 = normalized1;
    string finalText2 = normalized2;

    double score = jaccard_index(finalText1, finalText2);
    cout << score << endl;
    
    return 0;
}
