#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Pet {
protected:
    std::string name;
    std::string traits;
public:
    Pet(const std::string& name, const std::string& traits) : name(name), traits(traits) {}
    virtual ~Pet() {}
    virtual void Sound() const = 0;
    virtual void Type() const = 0;
    void Show() const {
        std::cout << "Ім'я: " << name << " (" << traits << ")" << std::endl;
    }
};

class Dog : public Pet {
public:
    Dog(const std::string& name, const std::string& traits) : Pet(name, traits) {}
    void Sound() const override { std::cout << name << ": Гав-гав!" << std::endl; }
    void Type() const override { std::cout << "Підвид: Собака" << std::endl; }
};

class Cat : public Pet {
public:
    Cat(const std::string& name, const std::string& traits) : Pet(name, traits) {}
    void Sound() const override { std::cout << name << ": Няв-няв!" << std::endl; }
    void Type() const override { std::cout << "Підвид: Кішка" << std::endl; }
};

class Parrot : public Pet {
public:
    Parrot(const std::string& name, const std::string& traits) : Pet(name, traits) {}
    void Sound() const override { std::cout << name << ": Кеша хороший!" << std::endl; }
    void Type() const override { std::cout << "Підвид: Папуга" << std::endl; }
};

class Hamster : public Pet {
public:
    Hamster(const std::string& name, const std::string& traits) : Pet(name, traits) {}
    void Sound() const override { std::cout << name << ": Піп-піп!" << std::endl; }
    void Type() const override { std::cout << "Підвид: Хом'як" << std::endl; }
};

int main() {
    std::vector<std::unique_ptr<Pet>> pets;
    pets.push_back(std::make_unique<Dog>("Рекс", "німецька вівчарка, 3 роки"));
    pets.push_back(std::make_unique<Cat>("Мурка", "сіра, пухнаста, 2 роки"));
    pets.push_back(std::make_unique<Parrot>("Кеша", "хвилястий, зелений, 1 рік"));
    pets.push_back(std::make_unique<Hamster>("Хомка", "джунгарський, 6 місяців"));

    for (const auto& p : pets) {
        p->Show();
        p->Type();
        p->Sound();
        std::cout << std::endl;
    }
    return 0;
}
