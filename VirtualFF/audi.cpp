#include <iostream>
#include <string>
#include <memory>
#include <limits>

using namespace std;

class Audio {
public:
    virtual void Play() const = 0;
    virtual ~Audio() = default;
};

class Song : public Audio {
private:
    string title;
    string artist;

public:
    Song(const string& title, const string& artist) : title(title), artist(artist) {}

    void Play() const override {
        cout << "Playing song: " << title << " by " << artist << endl;
    }
};

class Podcast : public Audio {
private:
    string host;
    string topic;

public:
    Podcast(const string& host, const string& topic) : host(host), topic(topic) {}

    void Play() const override {
        cout << "Podcast on " << topic << " hosted by " << host << endl;
    }
};

class Audiobook : public Audio {
private:
    string bookTitle;
    string author;
    string voice;

public:
    Audiobook(const string& bookTitle, const string& author, const string& voice)
        : bookTitle(bookTitle), author(author), voice(voice) {}

    void Play() const override {
        cout << "Listening to audiobook: " << bookTitle << " by " << author
             << ". Read by " << voice << "." << endl;
    }
};

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

int main() {
    int choice;

    do {
        cout << "\n=== Меню ===" << endl;
        cout << "1. Пісня (Song)" << endl;
        cout << "2. Подкаст (Podcast)" << endl;
        cout << "3. Аудіокнига (Audiobook)" << endl;
        cout << "0. Вихід" << endl;
        cout << "Оберіть тип аудіо: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        unique_ptr<Audio> audio;

        switch (choice) {
            case 1: {
                string title = readLine("Введіть назву: ");
                string artist = readLine("Введіть виконавця: ");
                audio = make_unique<Song>(title, artist);
                break;
            }
            case 2: {
                string host = readLine("Введіть ведучого: ");
                string topic = readLine("Введіть тему: ");
                audio = make_unique<Podcast>(host, topic);
                break;
            }
            case 3: {
                string bookTitle = readLine("Введіть назву книги: ");
                string author = readLine("Введіть автора: ");
                string voice = readLine("Введіть диктора: ");
                audio = make_unique<Audiobook>(bookTitle, author, voice);
                break;
            }
            case 0:
                cout << "Вихід." << endl;
                break;
            default:
                cout << "Невірний вибір." << endl;
        }

        if (audio) {
            audio->Play();
        }
    } while (choice != 0);

    return 0;
}
