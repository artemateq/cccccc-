#include <iostream>
#include <string>

void checkPassword(const std::string& password) {
    if (password.size() < 8) {
        throw std::invalid_argument("password is too short");
    }
    bool hasDigit = false;
    for (char c : password) {
        if (c >= '0' && c <= '9') {
            hasDigit = true;
        }
    }
    if (!hasDigit) {
        throw std::invalid_argument("password has no digit");
    }
}

int main()
{
    try {
        checkPassword("abcdefg1");
        std::cout << "password ok" << '\n';
        checkPassword("abcdefgh");
        std::cout << "password ok" << '\n';
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
