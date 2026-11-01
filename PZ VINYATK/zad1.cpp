#include <iostream>
#include <string>

int toInt(const std::string& s) {
    size_t pos = 0;
    int value = 0;
    try {
        value = std::stoi(s, &pos);
    }
    catch (...) {
        throw std::invalid_argument("not a number: " + s);
    }
    if (pos != s.size()) {
        throw std::invalid_argument("not a number: " + s);
    }
    return value;
}

int main()
{
    try {
        std::cout << toInt("123") << '\n';
        std::cout << toInt("-45") << '\n';
        std::cout << toInt("12abc") << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cout << "invalid argument block" << '\n';
        std::cout << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cout << "first block" << '\n';
        std::cout << e.what() << '\n';
    }
    catch (...) {
        std::cout << "unknown error\n";
    }
}
