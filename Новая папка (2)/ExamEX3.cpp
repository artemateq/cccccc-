
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


int DaysFromCivil(int y, int m, int d) {  // кількість днів від 1970-01-01
    y -= m <= 2;
    int era = (y >= 0 ? y : y - 399) / 400;
    int yoe = y - era * 400;
    int doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

std::string TodayString() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::ostringstream out;
    out << std::put_time(now, "%Y-%m-%d");
    return out.str();
}

// Перетворює "Year-Month-Day" у номер дня; повертає false, якщо формат поганий
bool ParseDate(const std::string& text, int& dayNumber) {
    int y, m, d;
    if (std::sscanf(text.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return false;
    if (m < 1 || m > 12 || d < 1 || d > 31) return false;
    dayNumber = DaysFromCivil(y, m, d);
    return true;
}

// Рахунки 
class Account {
public:
    explicit Account(const std::string& name) : name_(name) {}
    virtual ~Account() = default;

    virtual std::string GetType() const = 0;

    // Чи можна зняти суму (кожен тип рахунку має свої правила)
    virtual bool CanWithdraw(double amount) const { return balance_ >= amount; }

    void Deposit(double amount) { balance_ += amount; }
    bool Withdraw(double amount) {
        if (!CanWithdraw(amount)) return false;
        balance_ -= amount;
        return true;
    }

    const std::string& GetName() const { return name_; }
    double GetBalance() const { return balance_; }

protected:
    std::string name_;
    double balance_ = 0;
};

class Wallet : public Account {
public:
    using Account::Account;
    std::string GetType() const override { return "Wallet"; }
};

class DebitCard : public Account {
public:
    using Account::Account;
    std::string GetType() const override { return "Debit card"; }
};

class CreditCard : public Account {
public:
    CreditCard(const std::string& name, double limit) : Account(name), limit_(limit) {}
    std::string GetType() const override { return "Credit card"; }
    // На кредитці баланс може йти в мінус, але не глибше за ліміт
    bool CanWithdraw(double amount) const override { return balance_ - amount >= -limit_; }

private:
    double limit_;
};


struct Transaction {
    std::string date;      // "Year-Month-Day"
    int day = 0;           // номер дня (для порівнянь)
    bool isExpense = true;
    std::string account;
    std::string category;
    double amount = 0;
};



// уся логіка (рахунки, операції, звіти, рейтинги)

class FinanceManager {
public:
    void AddAccount(std::unique_ptr<Account> account) { accounts_.push_back(std::move(account)); }

    size_t AccountCount() const { return accounts_.size(); }

    std::string AccountsToString() const {
        std::ostringstream out;
        for (size_t i = 0; i < accounts_.size(); ++i) {
            out << i + 1 << ". " << accounts_[i]->GetName() << " (" << accounts_[i]->GetType()
                << ") balance: " << std::fixed << std::setprecision(2) << accounts_[i]->GetBalance() << "\n";
        }
        return out.str();
    }

    // поповнення рахунку
    void TopUp(size_t index, double amount) { accounts_[index]->Deposit(amount); }

    //витрати з рахунку 
    bool AddExpense(size_t index, const std::string& category, double amount, const std::string& date) {
        if (!accounts_[index]->Withdraw(amount)) return false;
        AddTransaction(index, category, amount, date, true);
        return true;
    }

    void AddIncome(size_t index, const std::string& category, double amount, const std::string& date) {
        accounts_[index]->Deposit(amount);
        AddTransaction(index, category, amount, date, false);
    }

    //Звіт за певний період: days = 1 (день), 7 (тиждень), 30 (місяць); період закінчується в endDay
    std::string BuildReport(int days, int endDay, const std::string& title) const {
        std::vector<Transaction> list = Filter(days, endDay, false);
        double income = 0, expense = 0;
        std::map<std::string, double> expenseByCategory;  // категорія -> сума витрат

        std::ostringstream out;
        out << "=== REPORT: " << title << " ===\n";
        for (const auto& t : list) {
            out << t.date << " | " << (t.isExpense ? "EXPENSE" : "INCOME ") << " | " << t.category
                << " | " << t.account << " | " << std::fixed << std::setprecision(2) << t.amount << "\n";
            if (t.isExpense) {
                expense += t.amount;
                expenseByCategory[t.category] += t.amount;
            }
            else {
                income += t.amount;
            }
        }
        out << "\nTotal income:  " << income << "\nTotal expense: " << expense << "\nExpenses by category:\n";
        for (const auto& pair : expenseByCategory) out << "  " << pair.first << ": " << pair.second << "\n";
        return out.str();
    }

    //3 найбільші витрати за період 
    std::string BuildTop3Expenses(int days, int endDay, const std::string& title) const {
        std::vector<Transaction> list = Filter(days, endDay, true);
        std::sort(list.begin(), list.end(),
            [](const Transaction& a, const Transaction& b) { return a.amount > b.amount; });

        std::ostringstream out;
        out << "=== TOP-3 EXPENSES: " << title << " ===\n";
        for (size_t i = 0; i < list.size() && i < 3; ++i) {
            out << i + 1 << ". " << list[i].category << " - " << std::fixed << std::setprecision(2)
                << list[i].amount << " (" << list[i].date << ")\n";
        }
        if (list.empty()) out << "No expenses.\n";
        return out.str();
    }

    //3 категорії за сумою витрат 
    std::string BuildTop3Categories(int days, int endDay, const std::string& title) const {
        std::map<std::string, double> sums;
        for (const auto& t : Filter(days, endDay, true)) sums[t.category] += t.amount;

        std::vector<std::pair<std::string, double>> sorted(sums.begin(), sums.end());
        std::sort(sorted.begin(), sorted.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

        std::ostringstream out;
        out << "=== TOP-3 CATEGORIES: " << title << " ===\n";
        for (size_t i = 0; i < sorted.size() && i < 3; ++i) {
            out << i + 1 << ". " << sorted[i].first << " - " << std::fixed << std::setprecision(2)
                << sorted[i].second << "\n";
        }
        if (sorted.empty()) out << "No expenses.\n";
        return out.str();
    }

private:
    std::vector<std::unique_ptr<Account>> accounts_;
    std::vector<Transaction> transactions_;

    void AddTransaction(size_t index, const std::string& category, double amount,
        const std::string& date, bool isExpense) {
        Transaction t;
        t.date = date;
        ParseDate(date, t.day);
        t.isExpense = isExpense;
        t.account = accounts_[index]->GetName();
        t.category = category;
        t.amount = amount;
        transactions_.push_back(t);
    }


    std::vector<Transaction> Filter(int days, int endDay, bool onlyExpenses) const {
        std::vector<Transaction> result;
        for (const auto& t : transactions_) {
            bool inPeriod = t.day > endDay - days && t.day <= endDay;
            if (inPeriod && (!onlyExpenses || t.isExpense)) result.push_back(t);
        }
        return result;
    }
};


std::string ReadLine(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

double ReadPositiveNumber(const std::string& prompt) {
    while (true) {
        try {
            double x = std::stod(ReadLine(prompt));
            if (x > 0) return x;
        }
        catch (...) {}
        std::cout << "Enter a number greater than 0.\n";
    }
}

// вибір рахунку зі списку
size_t ReadAccountIndex(const FinanceManager& manager) {
    std::cout << manager.AccountsToString();
    while (true) {
        try {
            int n = std::stoi(ReadLine("Account number: "));
            if (n >= 1 && n <= static_cast<int>(manager.AccountCount())) return n - 1;
        }
        catch (...) {}
        std::cout << "Wrong number.\n";
    }
}

// дата
std::string ReadDate() {
    while (true) {
        std::string s = ReadLine("Date YYYY-MM-DD (Enter = today): ");
        if (s.empty()) return TodayString();
        int dummy;
        if (ParseDate(s, dummy)) return s;
        std::cout << "Wrong date format.\n";
    }
}

//  період і дата яка є кінцем періоду
void ReadPeriod(int& days, int& endDay, std::string& title) {
    std::string choice = ReadLine("Period: 1 - day, 2 - week, 3 - month: ");
    days = (choice == "2") ? 7 : (choice == "3") ? 30 : 1;
    title = (days == 1) ? "day" : (days == 7) ? "week" : "month";
    std::string date = ReadDate();
    ParseDate(date, endDay);
    title += " ending " + date;
}


void ShowAndOfferSave(const std::string& text) {
    std::cout << "\n" << text << "\n";
    if (ReadLine("Save to file? (y/n): ") == "y") {
        std::ofstream out("reports.txt", std::ios::app);
        out << text << "\n";
        std::cout << "Saved to reports.txt\n";
    }
}

//тестові дані
void FillDemoData(FinanceManager& m) {
    m.AddAccount(std::make_unique<Wallet>("Cash"));
    m.AddAccount(std::make_unique<DebitCard>("Mono"));
    m.AddAccount(std::make_unique<CreditCard>("Credit", 5000));
    m.TopUp(0, 1000);
    m.TopUp(1, 5000);
    std::string today = TodayString();
    m.AddIncome(1, "Salary", 3000, today);
    m.AddExpense(0, "Food", 250, today);
    m.AddExpense(1, "Food", 400, today);
    m.AddExpense(1, "Transport", 120, today);
    m.AddExpense(2, "Games", 900, today);
    m.AddExpense(0, "Health", 600, today);
    std::cout << "Demo data added (dated today).\n";
}

int main() {
    FinanceManager manager;
    bool running = true;

    while (running) {
        std::cout << "\n===== PERSONAL FINANCE =====\n"
            << "1. Add wallet\n2. Add card\n3. Show accounts\n4. Top up account\n"
            << "5. Add expense\n6. Add income\n7. Report (day/week/month)\n"
            << "8. TOP-3 expenses\n9. TOP-3 categories\n10. Fill demo data\n0. Exit\n";
        std::string choice = ReadLine("Your choice: ");

        // Усі дії, які потребують рахунків
        bool needsAccounts = choice == "3" || choice == "4" || choice == "5" || choice == "6";
        if (needsAccounts && manager.AccountCount() == 0) {
            std::cout << "Create an account first.\n";
            continue;
        }

        if (choice == "1") {
            manager.AddAccount(std::make_unique<Wallet>(ReadLine("Wallet name: ")));
        }
        else if (choice == "2") {
            std::string name = ReadLine("Card name: ");
            if (ReadLine("Credit card? (y/n): ") == "y") {
                manager.AddAccount(std::make_unique<CreditCard>(name, ReadPositiveNumber("Credit limit: ")));
            }
            else {
                manager.AddAccount(std::make_unique<DebitCard>(name));
            }
        }
        else if (choice == "3") {
            std::cout << manager.AccountsToString();
        }
        else if (choice == "4") {
            size_t index = ReadAccountIndex(manager);
            manager.TopUp(index, ReadPositiveNumber("Amount: "));
        }
        else if (choice == "5") {
            size_t index = ReadAccountIndex(manager);
            std::string category = ReadLine("Category (e.g. Food): ");
            double amount = ReadPositiveNumber("Amount: ");
            if (!manager.AddExpense(index, category, amount, ReadDate())) std::cout << "Not enough money!\n";
        }
        else if (choice == "6") {
            size_t index = ReadAccountIndex(manager);
            std::string category = ReadLine("Category (e.g. Salary): ");
            double amount = ReadPositiveNumber("Amount: ");
            manager.AddIncome(index, category, amount, ReadDate());
        }
        else if (choice == "7" || choice == "8" || choice == "9") {
            int days, endDay;
            std::string title;
            if (choice != "7") {
                std::string p = ReadLine("Period: 1 - week, 2 - month: ");
                days = (p == "2") ? 30 : 7;
                std::string date = ReadDate();
                ParseDate(date, endDay);
                title = std::string(days == 7 ? "week" : "month") + " ending " + date;
            }
            else {
                ReadPeriod(days, endDay, title);
            }
            if (choice == "7") ShowAndOfferSave(manager.BuildReport(days, endDay, title));
            if (choice == "8") ShowAndOfferSave(manager.BuildTop3Expenses(days, endDay, title));
            if (choice == "9") ShowAndOfferSave(manager.BuildTop3Categories(days, endDay, title));
        }
        else if (choice == "10") {
            FillDemoData(manager);
        }
        else if (choice == "0") {
            running = false;
        }
        else {
            std::cout << "Unknown command.\n";
        }
    }
    return 0;
}
