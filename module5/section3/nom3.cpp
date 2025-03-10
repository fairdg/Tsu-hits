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
    
    stringstream ss(result);
    string word, normalized;
    while (ss >> word) {
        if (word.length() > 3) {
            if (!normalized.empty()) {
                normalized += " ";
            }
            normalized += word;
        }
    }
    
    return normalized;
}

double jaccard_coefficient(const string& str1, const string& str2) {
    set<char> set1(str1.begin(), str1.end());
    set<char> set2(str2.begin(), str2.end());
    
    int intersection_size = 0;
    for (char c : set1) {
        if (set2.count(c)) {
            intersection_size++;
        }
    }
    
    int union_size = set1.size() + set2.size() - intersection_size;
    
    if (union_size == 0) {
        return 0.0;
    } else {
        return static_cast<double>(intersection_size) / union_size;
    }
}

int main() {
    string s1, s2;
    cout << "Enter first string: ";
    getline(cin, s1);
    cout << "Enter second string: ";
    getline(cin, s2);
    
    s1 = normalize(s1);
    s2 = normalize(s2);
    
    double similarity = jaccard_coefficient(s1, s2);
    cout << "Jaccard Coefficient: " << fixed << setprecision(3) << similarity << endl;
    
    return 0;
}