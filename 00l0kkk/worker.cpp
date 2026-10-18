#include <iostream>
#include <string>
#include <ctime>

class Worker {
private:
    std::string fullName;
    std::string position;
    int hireYear;
    double salary;

public:
    explicit Worker(const std::string& fullName, const std::string& position,
                    int hireYear, double salary)
        : fullName(fullName), position(position),
          hireYear(hireYear), salary(salary) {}

    const std::string& getFullName() const { return fullName; }
    const std::string& getPosition() const { return position; }
    int getHireYear() const { return hireYear; }
    double getSalary() const { return salary; }

    int getExperience(int currentYear) const { return currentYear - hireYear; }

    void show() const {
        std::cout << fullName << " | " << position
                  << " | hired: " << hireYear
                  << " | salary: " << salary << std::endl;
    }
};

void showByExperience(const Worker workers[], int size, int currentYear, int years) {
    std::cout << "Workers with experience more than " << years << " years:" << std::endl;
    for (int i = 0; i < size; i++) {
        if (workers[i].getExperience(currentYear) > years) {
            workers[i].show();
        }
    }
    std::cout << std::endl;
}

void showBySalary(const Worker workers[], int size, double salary) {
    std::cout << "Workers with salary above " << salary << ":" << std::endl;
    for (int i = 0; i < size; i++) {
        if (workers[i].getSalary() > salary) {
            workers[i].show();
        }
    }
    std::cout << std::endl;
}

void showByPosition(const Worker workers[], int size, const std::string& position) {
    std::cout << "Workers with position \"" << position << "\":" << std::endl;
    for (int i = 0; i < size; i++) {
        if (workers[i].getPosition() == position) {
            workers[i].show();
        }
    }
    std::cout << std::endl;
}

int main() {
    std::time_t now = std::time(0);
    int currentYear = std::localtime(&now)->tm_year + 1900;

    Worker workers[] = {
        Worker("Ivanenko Ivan Ivanovych", "Manager", 2010, 35000),
        Worker("Petrenko Petro Petrovych", "Programmer", 2018, 52000),
        Worker("Sydorenko Olena Mykolaivna", "Accountant", 2005, 28000),
        Worker("Kovalenko Andrii Serhiiovych", "Programmer", 2022, 45000),
        Worker("Bondarenko Maria Oleksandrivna", "Manager", 2015, 40000),
        Worker("Tkachenko Oleh Viktorovych", "Director", 2001, 90000)
    };
    int size = sizeof(workers) / sizeof(workers[0]);

    std::cout << "All workers:" << std::endl;
    for (int i = 0; i < size; i++) {
        workers[i].show();
    }
    std::cout << std::endl;

    showByExperience(workers, size, currentYear, 10);
    showBySalary(workers, size, 40000);
    showByPosition(workers, size, "Programmer");

    return 0;
}
