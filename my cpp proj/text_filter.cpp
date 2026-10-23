#include <iostream>

int main() {
    std::string sentence;
    std::string badWord;

    std::cout << "Введіть речення: ";
    std::getline(std::cin, sentence);
    std::cout << "Введіть слово-паразит: ";
    std::getline(std::cin, badWord);

    if (!badWord.empty()) {
        std::size_t pos = sentence.find(badWord);
        if (pos != std::string::npos) {
            sentence.replace(pos, badWord.length(), "****");
        }
    }

    sentence += " [Відредаговано]";

    std::cout << sentence << "\n";

    return 0;
}
