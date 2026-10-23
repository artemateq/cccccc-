#include <iostream>

int main() {
    std::string email;
    std::cout << "Введіть електронну пошту: ";
    std::getline(std::cin, email);

    std::size_t atPos = email.find('@');
    if (atPos == std::string::npos) {
        std::cout << "Некоректна адреса: символ '@' не знайдено.\n";
        return 0;
    }

    std::string user = email.substr(0, atPos);
    std::string domain = email.substr(atPos + 1);

    std::cout << "Ім'я користувача: " << user << "\n";
    std::cout << "Домен: " << domain << "\n";

    return 0;
}
