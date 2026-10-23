#include <iostream>

template <typename T>
class Array {
    T* data;
    std::size_t size;
    std::size_t capacity;

public:
    Array() : data(nullptr), size(0), capacity(0) {}

    Array(const Array& other) : data(nullptr), size(0), capacity(0) {
        reserve(other.capacity);
        for (std::size_t i = 0; i < other.size; ++i) {
            data[i] = other.data[i];
        }
        size = other.size;
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            size = 0;
            capacity = 0;
            reserve(other.capacity);
            for (std::size_t i = 0; i < other.size; ++i) {
                data[i] = other.data[i];
            }
            size = other.size;
        }
        return *this;
    }

    ~Array() {
        delete[] data;
    }

    void reserve(std::size_t newCapacity) {
        if (newCapacity <= capacity) {
            return;
        }
        T* newData = new T[newCapacity];
        for (std::size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    void pushBack(T value) {
        if (size == capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

    bool empty() const {
        return size == 0;
    }

    void pushFront(T value) {
        if (size == capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        for (std::size_t i = size; i > 0; --i) {
            data[i] = data[i - 1];
        }
        data[0] = value;
        ++size;
    }

    void popFront() {
        if (empty()) {
            return;
        }
        for (std::size_t i = 1; i < size; ++i) {
            data[i - 1] = data[i];
        }
        --size;
    }

    std::size_t getSize() const {
        return size;
    }

    std::size_t getCapacity() const {
        return capacity;
    }

    T& operator[](std::size_t index) {
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        return data[index];
    }

    void print() const {
        for (std::size_t i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    Array<int> arr;
    std::cout << "empty: " << std::boolalpha << arr.empty() << "\n";

    arr.pushBack(10);
    arr.pushBack(20);
    arr.pushBack(30);
    arr.print();

    arr.pushFront(5);
    arr.pushFront(1);
    arr.print();
    std::cout << "size = " << arr.getSize() << ", capacity = " << arr.getCapacity() << "\n";

    arr.popFront();
    arr.print();
    arr.popFront();
    arr.popFront();
    arr.popFront();
    arr.popFront();
    std::cout << "empty: " << arr.empty() << "\n";
    arr.popFront();
    std::cout << "size = " << arr.getSize() << "\n";

    return 0;
}
