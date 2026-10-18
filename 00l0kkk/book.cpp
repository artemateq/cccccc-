#include <iostream>
#include <string>

class Book {
private:
    std::string author;
    std::string title;
    std::string publisher;
    int year;
    int quantity;
    int pages;

public:
    explicit Book(const std::string& author, const std::string& title,
                  const std::string& publisher, int year, int quantity, int pages)
        : author(author), title(title), publisher(publisher),
          year(year), quantity(quantity), pages(pages) {}

    const std::string& getAuthor() const { return author; }
    const std::string& getTitle() const { return title; }
    const std::string& getPublisher() const { return publisher; }
    int getYear() const { return year; }
    int getQuantity() const { return quantity; }
    int getPages() const { return pages; }

    void show() const {
        std::cout << author << " | " << title << " | " << publisher
                  << " | " << year << " | qty: " << quantity
                  << " | pages: " << pages << std::endl;
    }
};

void showByAuthor(const Book books[], int size, const std::string& author) {
    std::cout << "Books by author \"" << author << "\":" << std::endl;
    for (int i = 0; i < size; i++) {
        if (books[i].getAuthor() == author) {
            books[i].show();
        }
    }
    std::cout << std::endl;
}

void showByPublisher(const Book books[], int size, const std::string& publisher) {
    std::cout << "Books published by \"" << publisher << "\":" << std::endl;
    for (int i = 0; i < size; i++) {
        if (books[i].getPublisher() == publisher) {
            books[i].show();
        }
    }
    std::cout << std::endl;
}

void showAfterYear(const Book books[], int size, int year) {
    std::cout << "Books published after " << year << ":" << std::endl;
    for (int i = 0; i < size; i++) {
        if (books[i].getYear() > year) {
            books[i].show();
        }
    }
    std::cout << std::endl;
}

int main() {
    Book books[] = {
        Book("Taras Shevchenko", "Kobzar", "Dnipro", 1990, 5, 400),
        Book("Lesya Ukrainka", "Forest Song", "Osnova", 2005, 3, 180),
        Book("Taras Shevchenko", "Haidamaky", "Osnova", 2012, 2, 220),
        Book("Ivan Franko", "Zakhar Berkut", "Dnipro", 1998, 4, 310),
        Book("Lesya Ukrainka", "Stone Host", "Dnipro", 2018, 1, 150),
        Book("Ivan Franko", "Moses", "Osnova", 2020, 6, 260)
    };
    int size = sizeof(books) / sizeof(books[0]);

    std::cout << "All books:" << std::endl;
    for (int i = 0; i < size; i++) {
        books[i].show();
    }
    std::cout << std::endl;

    showByAuthor(books, size, "Taras Shevchenko");
    showByPublisher(books, size, "Osnova");
    showAfterYear(books, size, 2005);

    return 0;
}
