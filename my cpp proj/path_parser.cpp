#include <iostream>

int main() {
    std::string path;
    std::cout << "Введіть повний шлях до файлу: ";
    std::getline(std::cin, path);

    std::size_t slashPos = path.rfind('\\');
    std::string fileName;
    if (slashPos == std::string::npos) {
        fileName = path;
    } else {
        fileName = path.substr(slashPos + 1);
    }

    std::size_t dotPos = fileName.rfind('.');
    std::string name;
    std::string extension;
    if (dotPos == std::string::npos) {
        name = fileName;
        extension = "";
    } else {
        name = fileName.substr(0, dotPos);
        extension = fileName.substr(dotPos + 1);
    }

    std::cout << "Назва: " << name << "\n";
    std::cout << "Розширення: " << extension << "\n";

    return 0;
}
