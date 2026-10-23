#include <iostream>

int main() {
    std::string text;
    std::string word;

    std::cout << "Введіть текст: ";
    std::getline(std::cin, text);
    std::cout << "Введіть слово для пошуку: ";
    std::getline(std::cin, word);

    if (word.empty()) {
        std::cout << "Слово не може бути порожнім.\n";
        return 0;
    }

    int count = 0;
    std::size_t startPos = 0;
    std::size_t found = text.find(word, startPos);

    while (found != std::string::npos) {
        ++count;
        startPos = found + word.length();
        found = text.find(word, startPos);
    }

    std::cout << "Кількість входжень: " << count << "\n";

    return 0;
}
