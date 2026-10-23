#include <iostream>
#include <cstring>

class MyString {
    char* data;
    size_t length;

public:
    MyString() : data(new char[1]), length(0) {
        data[0] = '\0';
    }

    MyString(const char* str) {
        length = str ? std::strlen(str) : 0;
        data = new char[length + 1];
        if (str) {
            std::memcpy(data, str, length);
        }
        data[length] = '\0';
    }

    MyString(const MyString& other) : data(new char[other.length + 1]), length(other.length) {
        if (other.data) {
            std::memcpy(data, other.data, length);
        }
        data[length] = '\0';
        std::cout << "[Copy Constructor]\n";
    }

    MyString(MyString&& other) noexcept : data(other.data), length(other.length) {
        other.data = nullptr;
        other.length = 0;
        std::cout << "[Move Constructor]\n";
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) {
            char* newData = new char[other.length + 1];
            if (other.data) {
                std::memcpy(newData, other.data, other.length);
            }
            newData[other.length] = '\0';
            delete[] data;
            data = newData;
            length = other.length;
        }
        std::cout << "[Copy Assignment]\n";
        return *this;
    }

    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            length = other.length;
            other.data = nullptr;
            other.length = 0;
        }
        std::cout << "[Move Assignment]\n";
        return *this;
    }

    ~MyString() {
        delete[] data;
    }

    friend MyString operator+(const MyString& a, const MyString& b) {
        MyString result;
        delete[] result.data;
        result.length = a.length + b.length;
        result.data = new char[result.length + 1];
        if (a.length) {
            std::memcpy(result.data, a.data, a.length);
        }
        if (b.length) {
            std::memcpy(result.data + a.length, b.data, b.length);
        }
        result.data[result.length] = '\0';
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
        if (s.data) {
            os << s.data;
        }
        return os;
    }
};

int main() {
    MyString a("Hello, ");
    MyString b("World!");

    MyString c = a + b;
    std::cout << "c = " << c << "\n";

    MyString d = std::move(c);
    std::cout << "d = " << d << "\n";
    std::cout << "c (after move) = '" << c << "'\n";

    MyString e;
    e = std::move(d);
    std::cout << "e = " << e << "\n";

    MyString f;
    f = a + b;
    std::cout << "f = " << f << "\n";

    MyString g = a;
    std::cout << "g = " << g << "\n";

    return 0;
}
