#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    char* name;
    int health;
    int level;

    static int totalPlayers;

public:
    Player(const char* name, int health, int level);
    Player();
    ~Player();

    const char* getName() const;
    int getHealth() const;
    int getLevel() const;

    void setName(const char* newName);
    void setHealth(int newHealth);

    void takeDamage(int damage);
    void heal(int amount);
    void levelUp();
    void printStats() const;

    static int getTotalPlayers();
};

#endif
