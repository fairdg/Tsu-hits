#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main() {
    string str;
    
    string DDMMfeb = R"(([0-2][0-9])\.(02))";
    string MMDDfeb = R"((02)\.([0-2][0-9]))";
    string DDMM = R"(([012][0-9]|3[01])\.([0][3-9]|1[012]|0[1]))";
    string MMDD = R"(([0][3-9]|1[012]|0[1])\.([012][0-9]|3[01]))";
    string year = R"(\.([0-9]{2}([0-9]{2})?))";
    string sum = DDMMfeb + year + "|" + MMDDfeb + year + "|" + DDMM + year + "|" + MMDD + year;

    
    regex data(sum);
    while (getline(cin, str)) {
        smatch match;
        string temp = str;

        while (regex_search(temp, match, data)) {
            cout << match.str() << endl;
            temp = match.suffix().str();
        }
    }

    return 0;
}
