#include <iostream>
#include <string>

class IHackable {
public:
    virtual void applyHack(int techLevel) = 0;
    virtual bool isHacked() const = 0;
    virtual ~IHackable() {}
};

class VendingMachine : public IHackable {
    bool hacked = false;
public:
    void applyHack(int techLevel) override {
        if (techLevel >= 2) {
            hacked = true;
            std::cout << "Автомат зламано! Він викидає всі напої: кола, фанта, спрайт, мінералка." << std::endl;
        } else {
            std::cout << "Автомат б'є хакера струмом!" << std::endl;
        }
    }
    bool isHacked() const override { return hacked; }
};

class SmartDoor : public IHackable {
    bool open = false;
public:
    void applyHack(int techLevel) override {
        if (techLevel >= 5) {
            open = true;
            std::cout << "Двері відчинено." << std::endl;
        } else {
            std::cout << "Двері залишилися зачиненими." << std::endl;
        }
    }
    bool isHacked() const override { return open; }
};

class CyberEnemy {
protected:
    int health;
public:
    explicit CyberEnemy(int health) : health(health) {}
    virtual ~CyberEnemy() {}
    virtual void attack() = 0;
    int getHealth() const { return health; }
};

class CyborgGuard : public CyberEnemy, public IHackable {
    bool frozen = false;
public:
    explicit CyborgGuard(int health = 100) : CyberEnemy(health) {}
    void attack() override {
        if (frozen) {
            std::cout << "Кіборг заморожений і не може атакувати." << std::endl;
        } else {
            std::cout << "Кіборг атакує гравця!" << std::endl;
        }
    }
    void applyHack(int techLevel) override {
        if (techLevel >= 7) {
            frozen = true;
            std::cout << "Кіборга вимкнено (стан: заморожений)." << std::endl;
        } else {
            health -= 20;
            std::cout << "Вірус слабкий: кіборг втрачає 20 здоров'я. Залишилось: " << health << std::endl;
        }
    }
    bool isHacked() const override { return frozen; }
};

class AutoTurret : public CyberEnemy, public IHackable {
    bool hacked = false;
public:
    explicit AutoTurret(int health = 80) : CyberEnemy(health) {}
    void attack() override {
        if (hacked) {
            std::cout << "Турель стріляє по іншим ворогам!" << std::endl;
        } else {
            std::cout << "Турель стріляє по гравцю!" << std::endl;
        }
    }
    void applyHack(int techLevel) override {
        if (techLevel >= 4) {
            hacked = true;
            std::cout << "Турель змінила ціль з гравця на інших ворогів." << std::endl;
        } else {
            std::cout << "Злам турелі не вдався." << std::endl;
        }
    }
    bool isHacked() const override { return hacked; }
};

class Netrunner {
    int skillLevel;
public:
    explicit Netrunner(int skillLevel) : skillLevel(skillLevel) {}
    void breachProtocol(IHackable& target) {
        target.applyHack(this->skillLevel);
    }
};

int main() {
    Netrunner hacker(5);

    SmartDoor door;
    VendingMachine machine;
    CyborgGuard cyborg;

    hacker.breachProtocol(door);
    hacker.breachProtocol(machine);
    hacker.breachProtocol(cyborg);

    std::cout << std::endl;
    std::cout << "Двері зламано: " << (door.isHacked() ? "так" : "ні") << std::endl;
    std::cout << "Автомат зламано: " << (machine.isHacked() ? "так" : "ні") << std::endl;
    std::cout << "Кіборг вимкнений: " << (cyborg.isHacked() ? "так" : "ні")
              << ", здоров'я: " << cyborg.getHealth() << std::endl;

    return 0;
}
