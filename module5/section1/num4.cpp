#include <iostream>
#include <regex>

bool isValidBracketSequence(const std::string& s) {
    std::regex re("^(\"|\(|\)|\{|\})*$");
    if (!std::regex_match(s, re)) return false;
    
    std::string temp = s;
    std::regex pattern("\(\)|\{\}");
    while (std::regex_search(temp, pattern)) {
        temp = std::regex_replace(temp, pattern, "");
    }
    return temp.empty();
}

int main() {
    std::string input;
    std::cout << "Введите скобочную последовательность: ";
    std::cin >> input;
    
    if (isValidBracketSequence(input)) {
        std::cout << "Правильная скобочная последовательность" << std::endl;
    } else {
        std::cout << "Неправильная скобочная последовательность" << std::endl;
    }
    
    return 0;
}
