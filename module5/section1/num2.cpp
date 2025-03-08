#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main() {
    string str1;
    regex time(R"(((0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]))|((0?[1-9]|1[0-2]):([0-5][0-9])))");

    while (getline(cin, str1)) {
        smatch match;
        string temp = str1;

        while (regex_search(temp, match, time)) {
            cout << match.str() << endl;
            temp = match.suffix().str();
        }
    }
    return 0;
}
