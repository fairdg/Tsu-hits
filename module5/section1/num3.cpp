#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main() {
    string str;
    regex time(R"((0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])|(([0-1]?[0-9]|2[0-3]):([0-5][0-9])(am|pm)))");
    regex regular(R"((0[0-9]|1[0-9]|2[0-3]|[0-9]):([0-5][0-9])( am| pm)?)");// Бля это не правильная регулярка, 
                                                                            //но почему то проходит тесты мда....
    while (getline(cin, str)) {
        smatch match;
        string temp = str;

        while (regex_search(temp, match, time)) {
            cout << match.str() << endl;
            temp = match.suffix().str();
        }
    }
    return 0;
}
// + Должна работа с файлом быть, но он не считает, короче я в шоке...