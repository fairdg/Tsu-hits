#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main() {
    string str;
    regex reg(R"((\w+\s*=\s*\d+;)|(\w+\s*=\s*\w+;))");

    while (getline(cin, str)) {
        smatch match;
        string temp = str;

        while (regex_search(temp, match, reg)) {
            cout << match.str() << endl;
            temp = match.suffix().str();
        }
    }
    return 0;
}
