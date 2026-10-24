#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

std::string toLower(const std::string& s) {
    std::string r;
    r.reserve(s.size());

    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);

        if (c < 128) {
            r += static_cast<char>(std::tolower(c));
        } else if (c == 0xD0 && i + 1 < s.size()) {
            unsigned char d = static_cast<unsigned char>(s[++i]);
            if (d >= 0x90 && d <= 0x9F) {
                r += static_cast<char>(0xD0);
                r += static_cast<char>(d + 0x20);
            } else if (d >= 0xA0 && d <= 0xAF) {
                r += static_cast<char>(0xD1);
                r += static_cast<char>(d - 0x20);
            } else if (d >= 0x80 && d <= 0x8F) {
                r += static_cast<char>(0xD1);
                r += static_cast<char>(d + 0x10);
            } else {
                r += static_cast<char>(c);
                r += static_cast<char>(d);
            }
        } else if (c == 0xD2 && i + 1 < s.size()) {
            unsigned char d = static_cast<unsigned char>(s[++i]);
            r += static_cast<char>(c);
            r += static_cast<char>(d == 0x90 ? 0x91 : d);
        } else {
            r += static_cast<char>(c);
        }
    }
    return r;
}

bool isWordChar(unsigned char c) {
    return std::isalnum(c) || c >= 128;
}

std::string cleanWord(const std::string& w) {
    size_t b = 0, e = w.size();
    while (b < e && !isWordChar(static_cast<unsigned char>(w[b]))) ++b;
    while (e > b) {
        unsigned char c = static_cast<unsigned char>(w[e - 1]);
        if (isWordChar(c) || c == '+' || c == '#') break;
        --e;
    }
    return w.substr(b, e - b);
}

int main() {
    std::map<std::string, int> freq;
    std::string line;

    while (std::getline(std::cin, line) && !line.empty()) {
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            std::string word = cleanWord(toLower(token));
            if (!word.empty()) ++freq[word];
        }
    }

    if (freq.empty()) return 0;

    int maxCount = 0;
    for (const auto& [word, count] : freq) {
        std::cout << word << " -> " << count << "\n";
        if (count > maxCount) maxCount = count;
    }

    std::cout << "\n";
    for (const auto& [word, count] : freq) {
        if (count == maxCount) std::cout << word << " -> " << count << "\n";
    }

    return 0;
}
