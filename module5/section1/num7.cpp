#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main() {
    string str;
    int str_number = 0;

    while (getline(cin, str)) {
        str_number++;
        if (str.find("Merlin speaks") == 0) {
            regex speach("Avada-ke-davra");
            smatch match;
            if (regex_search(str, match, speach)) {
                cout << str_number << " " << match.position() + 1 << endl;
            }
        }
    }

    return 0;
}