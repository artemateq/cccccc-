#include <iostream>
#include <string>

class AgeException : public std::exception {
    int age;
    std::string message;
public:
    AgeException(int age) : age(age) {
        message = "Age error: " + std::to_string(age) + ", must be 18+";
    }
    ~AgeException() = default;

    int getAge() const { return age; }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

void vote(std::string candidate, int age) {
    if (age < 18) {
        throw AgeException(age);
    }
    std::cout << "Voted for " << candidate << '\n';
}

int main()
{
    try {
        vote("Ivanenko", 20);
        vote("Petrenko", 16);
    }
    catch (const AgeException& e) {
        std::cout << "age exception block" << '\n';
        std::cout << e.what() << '\n';
        std::cout << "age: " << e.getAge() << '\n';
    }
    catch (const std::exception& e) {
        std::cout << "first block" << '\n';
        std::cout << e.what() << '\n';
    }
    catch (...) {
        std::cout << "unknown error\n";
    }
}
