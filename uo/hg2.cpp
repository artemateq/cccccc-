#include <iostream>
#include <fstream>
#include <random>

class Matrix {
    int rows;
    int cols;
    int** data;

    static std::mt19937& engine() {
        static std::mt19937 gen{std::random_device{}()};
        return gen;
    }

    void allocate(int r, int c) {
        rows = r;
        cols = c;
        data = new int*[rows];
        for (int i = 0; i < rows; ++i)
            data[i] = new int[cols]{};
    }

    void release() {
        for (int i = 0; i < rows; ++i)
            delete[] data[i];
        delete[] data;
        data = nullptr;
        rows = cols = 0;
    }

public:
    Matrix() : rows(0), cols(0), data(nullptr) {}

    Matrix(int r, int c) : rows(0), cols(0), data(nullptr) {
        allocate(r, c);
        std::uniform_int_distribution<int> dist(0, 99);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] = dist(engine());
    }

    Matrix(const Matrix& other) : rows(0), cols(0), data(nullptr) {
        allocate(other.rows, other.cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] = other.data[i][j];
    }

    Matrix& operator=(const Matrix& other) {
        if (this == &other) return *this;
        release();
        allocate(other.rows, other.cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] = other.data[i][j];
        return *this;
    }

    ~Matrix() { release(); }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        os << m.rows << ' ' << m.cols << '\n';
        for (int i = 0; i < m.rows; ++i) {
            for (int j = 0; j < m.cols; ++j)
                os << m.data[i][j] << ' ';
            os << '\n';
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Matrix& m) {
        int r, c;
        if (!(is >> r >> c) || r <= 0 || c <= 0) {
            is.setstate(std::ios::failbit);
            return is;
        }
        Matrix tmp;
        tmp.allocate(r, c);
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                if (!(is >> tmp.data[i][j]))
                    return is;
        m = tmp;
        return is;
    }
};

int main() {
    Matrix random(3, 4);
    std::cout << "Випадкова матриця (консоль):\n" << random << '\n';

    {
        std::ofstream out("matrix.txt");
        if (!out) {
            std::cout << "Не вдалося відкрити matrix.txt для запису\n";
            return 1;
        }
        out << random;
    }
    std::cout << "Матрицю записано у файл matrix.txt\n\n";

    Matrix fromFile;
    {
        std::ifstream in("matrix.txt");
        if (!(in >> fromFile)) {
            std::cout << "Не вдалося прочитати матрицю з файлу\n";
            return 1;
        }
    }
    std::cout << "Матриця, прочитана з файлу:\n" << fromFile << '\n';

    std::cout << "Введіть матрицю з консолі (спочатку рядки та стовпці, потім елементи):\n";
    Matrix fromConsole;
    if (!(std::cin >> fromConsole)) {
        std::cout << "Помилка введення матриці\n";
        return 1;
    }
    std::cout << "Введена матриця:\n" << fromConsole;

    return 0;
}
