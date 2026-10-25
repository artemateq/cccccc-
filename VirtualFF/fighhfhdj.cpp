#include <iostream>
#include <memory>

using namespace std;

const double PI = 3.14159265358979323846;

class Figure {
public:
    virtual double GetArea() const = 0;
    virtual ~Figure() = default;
};

class Rectangle : public Figure {
private:
    double width;
    double height;

public:
    Rectangle(double width, double height) : width(width), height(height) {}

    double GetArea() const override {
        return width * height;
    }
};

class Circle : public Figure {
private:
    double radius;

public:
    Circle(double radius) : radius(radius) {}

    double GetArea() const override {
        return PI * radius * radius;
    }
};

class Triangle : public Figure {
private:
    double base;
    double height;

public:
    Triangle(double base, double height) : base(base), height(height) {}

    double GetArea() const override {
        return 0.5 * base * height;
    }
};

int main() {
    int choice;

    do {
        cout << "\n=== Меню ===" << endl;
        cout << "1. Прямокутник" << endl;
        cout << "2. Коло" << endl;
        cout << "3. Трикутник" << endl;
        cout << "0. Вихід" << endl;
        cout << "Оберіть фігуру: ";
        cin >> choice;

        unique_ptr<Figure> figure;

        switch (choice) {
            case 1: {
                double w, h;
                cout << "Введіть ширину: ";
                cin >> w;
                cout << "Введіть висоту: ";
                cin >> h;
                figure = make_unique<Rectangle>(w, h);
                break;
            }
            case 2: {
                double r;
                cout << "Введіть радіус: ";
                cin >> r;
                figure = make_unique<Circle>(r);
                break;
            }
            case 3: {
                double b, h;
                cout << "Введіть основу: ";
                cin >> b;
                cout << "Введіть висоту: ";
                cin >> h;
                figure = make_unique<Triangle>(b, h);
                break;
            }
            case 0:
                cout << "Вихід." << endl;
                break;
            default:
                cout << "Невірний вибір." << endl;
        }

        if (figure) {
            cout << "Площа: " << figure->GetArea() << endl;
        }
    } while (choice != 0);

    return 0;
}
