#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main() {
    string str;
    
    string reg1 = R"([a-zA-Z_]\w*\s*=\s*-?\d+\s*;)";
    string reg2 = R"([a-zA-Z_]\w*\s*=\s*[a-zA-Z_]\w*\s*;)";
    string reg3 = R"([a-zA-Z_]\w*\s*=\s*\[[a-zA-Z_]\w*|\d+\]\s*([+\-*/^]\s*([a-zA-Z_]\w*|\d+|\[[a-zA-Z_]\w*|\d+\]))+;)";




    
    string save = reg1 + "|" + reg2 + "|" + reg3 ;
    regex reg(save);

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