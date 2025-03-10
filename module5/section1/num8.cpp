#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main() {
    string str;
    regex expression(R"(([1-9][0-9]*((\+|\*|\/|\-)[1-9][0-9]*)+)|[1-9][0-9]*)");
    while (getline(cin, str)) {
        smatch match;
        string temp = str;

        while (regex_search(temp, match, expression)) {
            cout << match.str() << endl;
            temp = match.suffix().str();
        }
    }
    return 0;
}
 