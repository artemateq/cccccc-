#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

struct GameRecord {
    int target;
    int attempts;
};

class GameStorage {
    std::string filename;
public:
    explicit GameStorage(const std::string& file) : filename(file) {}

    void save(const GameRecord& record) const {
        std::ofstream out(filename, std::ios::app);
        if (!out) {
            std::cout << "Не вдалося зберегти гру у файл " << filename << '\n';
            return;
        }
        out << record.target << ' ' << record.attempts << '\n';
    }

    std::vector<GameRecord> load() const {
        std::vector<GameRecord> records;
        std::ifstream in(filename);
        if (!in) return records;

        GameRecord r;
        while (in >> r.target >> r.attempts)
            records.push_back(r);
        return records;
    }
};

void clearInput() {
    std::cin.clear();
    char c;
    while (std::cin.get(c) && c != '\n') {}
}

class Game {
    static const int MIN_NUMBER = 1;
    static const int MAX_NUMBER = 500;

    std::mt19937 gen{std::random_device{}()};

public:
    GameRecord play() {
        std::uniform_int_distribution<int> dist(MIN_NUMBER, MAX_NUMBER);
        int target = dist(gen);
        int attempts = 0;

        std::cout << "Я загадав число від " << MIN_NUMBER << " до " << MAX_NUMBER
                  << ". Спробуйте вгадати!\n";

        while (true) {
            std::cout << "Ваша здогадка: ";
            int guess;
            if (!(std::cin >> guess)) {
                if (std::cin.eof()) return {target, -1};
                clearInput();
                std::cout << "Введіть ціле число\n";
                continue;
            }

            ++attempts;
            if (guess < target)
                std::cout << "Загадане число більше\n";
            else if (guess > target)
                std::cout << "Загадане число менше\n";
            else {
                std::cout << "Вгадали! Кількість спроб: " << attempts << '\n';
                return {target, attempts};
            }
        }
    }
};

void showStats(const GameStorage& storage) {
    std::vector<GameRecord> records = storage.load();
    if (records.empty()) {
        std::cout << "Ігор ще не було\n";
        return;
    }

    int best = records[0].attempts;
    int worst = records[0].attempts;
    long long total = 0;

    std::cout << "\n№\tЧисло\tСпроби\n";
    for (std::size_t i = 0; i < records.size(); ++i) {
        std::cout << i + 1 << '\t' << records[i].target << '\t'
                  << records[i].attempts << '\n';
        total += records[i].attempts;
        if (records[i].attempts < best) best = records[i].attempts;
        if (records[i].attempts > worst) worst = records[i].attempts;
    }

    std::cout << "\nВсього ігор: " << records.size()
              << "\nНайкращий результат: " << best << " спроб"
              << "\nНайгірший результат: " << worst << " спроб"
              << "\nУ середньому: ";
    std::cout.setf(std::ios::fixed);
    std::cout.precision(1);
    std::cout << static_cast<double>(total) / records.size() << " спроб\n";
    std::cout.unsetf(std::ios::fixed);
}

int main() {
    GameStorage storage("games.txt");
    Game game;

    while (true) {
        std::cout << "\n1 - Нова гра\n2 - Статистика\n0 - Вихід\nВибір: ";
        int choice;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;
            clearInput();
            std::cout << "Невірний вибір\n";
            continue;
        }

        if (choice == 1) {
            GameRecord record = game.play();
            if (record.attempts < 0) break;
            storage.save(record);
        } else if (choice == 2) {
            showStats(storage);
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Невірний вибір\n";
        }
    }

    return 0;
}
