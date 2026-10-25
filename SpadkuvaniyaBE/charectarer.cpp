#include <iostream>
#include <string>

using namespace std;

class Character {
protected:
    string name;
    int health;
    int shield;

public:
    Character(const string& name) : name(name), health(100), shield(50) {}

    void takeDamage(int amount) {
        if (amount <= 0) return;

        if (shield >= amount) {
            shield -= amount;
        } else {
            int remainder = amount - shield;
            shield = 0;
            health -= remainder;
            if (health < 0) health = 0;
        }
    }

    void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;
    }

    string getName() const { return name; }
    int getHealth() const { return health; }
    int getShield() const { return shield; }

    void printStatus() const {
        cout << name << ": здоров'я = " << health << ", броня = " << shield << endl;
    }

    virtual ~Character() = default;
};

class Recon : public Character {
public:
    Recon(const string& name) : Character(name) {}

    void scanArea() const {
        cout << name << " сканує місцевість: виявлено 2 ворогів!" << endl;
    }
};

class Support : public Character {
public:
    Support(const string& name) : Character(name) {}

    void dropHeal(Character& ally, int amount = 30) const {
        ally.heal(amount);
        cout << name << " скидає аптечку: " << ally.getName() << " відновлює " << amount << " здоров'я." << endl;
    }
};

int main() {
    Recon recon("Тінь");
    Support support("Лікар");

    recon.takeDamage(30);
    recon.printStatus();

    support.takeDamage(80);
    support.printStatus();

    recon.scanArea();
    support.dropHeal(support, 25);
    support.printStatus();

    return 0;
}
