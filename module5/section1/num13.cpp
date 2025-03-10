#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main() {
    string str1;
    string time1 = R"(\b((0[1-9]|[12][0-9]|31)\.(0[1-9]|1[0-2])\.(\d{2}|\d{4})|(0[1-9]|[12][0-9])\.02\.(\d{2}|\d{4}))\b)";
    string time2 = R"(\b((0[1-9]|1[0-2])\.(0[1-9]|[12][0-9]|31)\.(\d{2}|\d{4})|02\.(0[1-9]|[12][0-9])\.(\d{2}|\d{4}))\b)";

    string time3 = time1 + "|" + time2;

    regex time(time3);

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
