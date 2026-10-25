#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Employer {
protected:
    std::string name;
    double salary;
public:
    Employer(const std::string& name, double salary) : name(name), salary(salary) {}
    virtual ~Employer() {}
    virtual void Print() const = 0;
};

class President : public Employer {
    std::string country;
public:
    President(const std::string& name, double salary, const std::string& country)
        : Employer(name, salary), country(country) {}
    void Print() const override {
        std::cout << "Президент: " << name << ", країна: " << country
                  << ", зарплата: " << salary << std::endl;
    }
};

class Manager : public Employer {
    std::string department;
    int subordinates;
public:
    Manager(const std::string& name, double salary, const std::string& department, int subordinates)
        : Employer(name, salary), department(department), subordinates(subordinates) {}
    void Print() const override {
        std::cout << "Менеджер: " << name << ", відділ: " << department
                  << ", підлеглих: " << subordinates << ", зарплата: " << salary << std::endl;
    }
};

class Worker : public Employer {
    std::string profession;
    int hoursPerWeek;
public:
    Worker(const std::string& name, double salary, const std::string& profession, int hoursPerWeek)
        : Employer(name, salary), profession(profession), hoursPerWeek(hoursPerWeek) {}
    void Print() const override {
        std::cout << "Працівник: " << name << ", професія: " << profession
                  << ", годин на тиждень: " << hoursPerWeek << ", зарплата: " << salary << std::endl;
    }
};

int main() {
    std::vector<std::unique_ptr<Employer>> staff;
    staff.push_back(std::make_unique<President>("Іван Петренко", 100000, "Україна"));
    staff.push_back(std::make_unique<Manager>("Олена Коваль", 50000, "Продажі", 12));
    staff.push_back(std::make_unique<Worker>("Андрій Шевченко", 20000, "Слюсар", 40));

    for (const auto& e : staff) {
        e->Print();
    }
    return 0;
}
