#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;

string normalize(const string& str) {
    string result;
    for (char c : str) {
        if (isalnum(c) || c == ' ') {
            result += tolower(c);
        }
    }
    stringstream ss(result);
    string word, word1;
    while (ss >> word) {
        if (word.length() > 3) {
            if (!word1.empty()) {
                word1 += " ";
            }
            word1 += word;
        }
    }

    return word1;
}

int leven(const string& str1, const string& str2) {
    int n = str1.length();
    int m = str2.length();
    
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int cost;
            if (str1[i - 1] == str2[j - 1]) {
                cost = 0;
            } else {
                cost = 1;
            }

            if (dp[i - 1][j] + 1 < dp[i][j - 1] + 1) {
                if (dp[i - 1][j] + 1 < dp[i - 1][j - 1] + cost) {
                    dp[i][j] = dp[i - 1][j] + 1;
                } else {
                    dp[i][j] = dp[i - 1][j - 1] + cost;
                }
            } else {
                if (dp[i][j - 1] + 1 < dp[i - 1][j - 1] + cost) {
                    dp[i][j] = dp[i][j - 1] + 1;
                } else {
                    dp[i][j] = dp[i - 1][j - 1] + cost;
                }
            }
        }
    }
    
    return dp[n][m];
}

int main() {
    string s1, s2;
    getline(cin, s1);
    getline(cin, s2);
    s1 = normalize(s1);
    s2 = normalize(s2);
    cout << leven(s1, s2) << endl;

    return 0;
}
