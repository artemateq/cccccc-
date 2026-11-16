
#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

const std::string kWordsFile = "words.dat";        
const std::string kStatsFile = "hangman_stats.txt"; 
const int kMaxMistakes = 6;                        


public:
    static std::string Encrypt(const std::string& text) {
        std::ostringstream out;
        for (size_t i = 0; i < text.size(); ++i) {
            unsigned char c = text[i] ^ kKey[i % kKey.size()];
            out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        }
        return out.str();
    }

    static std::string Decrypt(const std::string& hex) {
        std::string text;
        for (size_t i = 0; i + 1 < hex.size(); i += 2) {
            int byte = std::stoi(hex.substr(i, 2), nullptr, 16);
            text += static_cast<char>(byte ^ kKey[(i / 2) % kKey.size()]);
        }
        return text;
    }

private:
    inline static const std::string kKey = "S3cr3tK3y";
};

class WordList {
public:
    WordList() {
        CreateFileIfMissing();
        LoadFromFile();
    }

    //випадкове слово
    std::string GetRandomWord() const {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, words_.size() - 1);
        return words_[dist(gen)];
    }

private:
    std::vector<std::string> words_;

    void CreateFileIfMissing() const {
        std::ifstream check(kWordsFile);
        if (check) return;

        std::vector<std::string> defaults = { "program", "computer", "keyboard", "algorithm", "variable",
        "compiler", "network", "function", "database", "software",
            "elephant", "giraffe", "penguin", "dolphin", "butterfly",
            "kangaroo", "crocodile", "squirrel", "octopus", "hedgehog",
            "chocolate", "pancake", "strawberry", "sandwich", "pineapple",
            "mushroom", "broccoli", "cinnamon", "avocado", "pumpkin",
            "mountain", "waterfall", "volcano", "island", "desert",
            "rainbow", "thunder", "forest", "glacier", "harbor",
            "umbrella", "telescope", "treasure", "adventure", "bicycle",
            "guitar", "lantern", "puzzle", "journey", "hangman" };
        std::ofstream out(kWordsFile);
        for (const auto& word : defaults) out << Cipher::Encrypt(word) << "\n";
    }

    void LoadFromFile() {
        std::ifstream in(kWordsFile);
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) words_.push_back(Cipher::Decrypt(line));
        }
    }
};


struct GameStats {
    double seconds = 0;           
    int attempts = 0;             
    std::string word;             
    std::vector<char> letters;    
    bool won = false;

    std::string ToString() const {
        std::ostringstream out;
        out << "Result: " << (won ? "WIN" : "LOSE") << "\n"
            << "Time (sec): " << std::fixed << std::setprecision(1) << seconds << "\n"
            << "Attempts: " << attempts << "\n"
            << "Word: " << word << "\n"
            << "Player letters: ";
        for (char c : letters) out << c << ' ';
        out << "\n";
        return out.str();
    }

   
    void SaveToFile() const {
        std::ofstream out(kStatsFile, std::ios::app);
        out << ToString() << "-----------------\n";
    }
};


class HangmanGame {
public:
    explicit HangmanGame(const std::string& word) : secret_(word) {}

    GameStats Play() {
        auto start = std::chrono::steady_clock::now();
        GameStats stats;
        stats.word = secret_;

        while (mistakes_ < kMaxMistakes && !IsWordGuessed()) {
            Draw();
            char letter = ReadLetter();
            ++stats.attempts;
            stats.letters.push_back(letter);
            guessed_.push_back(letter);

            if (secret_.find(letter) == std::string::npos) {
                ++mistakes_;
                std::cout << "No such letter!\n";
            }
        }

        stats.won = IsWordGuessed();
        Draw();
        stats.seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        return stats;
    }

private:
    std::string secret_;
    std::vector<char> guessed_;
    int mistakes_ = 0;

    bool IsGuessed(char c) const {
        return std::find(guessed_.begin(), guessed_.end(), c) != guessed_.end();
    }

    bool IsWordGuessed() const {
        return std::all_of(secret_.begin(), secret_.end(), [this](char c) { return IsGuessed(c); });
    }

    // Читаємо одну нову літеру (повторні та не-літери відхиляємо)
    char ReadLetter() const {
        std::string input;
        while (true) {
            std::cout << "Enter a letter: ";
            std::getline(std::cin, input);
            if (input.size() == 1 && std::isalpha(static_cast<unsigned char>(input[0]))) {
                char c = static_cast<char>(std::tolower(static_cast<unsigned char>(input[0])));
                if (!IsGuessed(c)) return c;
                std::cout << "You already tried this letter.\n";
            } else {
                std::cout << "Please enter exactly one letter.\n";
            }
        }
    }

    
    void Draw() const {
        const char* parts = "O|/\\/\\";  
        char p[6];
        for (int i = 0; i < 6; ++i) p[i] = (mistakes_ > i) ? parts[i] : ' ';

        std::cout << "\n  +---+\n"
                  << "  |   " << p[0] << "\n"
                  << "  |  " << p[2] << p[1] << p[3] << "\n"
                  << "  |  " << p[4] << " " << p[5] << "\n"
                  << "  |\n=====\n";

        std::cout << "Word: ";
        for (char c : secret_) std::cout << (IsGuessed(c) ? c : '_') << ' ';
        std::cout << "\nMistakes: " << mistakes_ << "/" << kMaxMistakes << "\n";
    }
};

int main() {
    WordList wordList;
    std::string again = "y";

    while (again == "y" || again == "Y") {
        HangmanGame game(wordList.GetRandomWord());
        GameStats stats = game.Play();

        std::cout << "\n===== GAME OVER =====\n" << stats.ToString();
        stats.SaveToFile();
        std::cout << "(statistics saved to " << kStatsFile << ")\n";

        std::cout << "\nPlay again? (y/n): ";
        std::getline(std::cin, again);
    }
    return 0;
}
