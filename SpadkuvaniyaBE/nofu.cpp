#include <iostream>
#include <string>
#include <ctime>
#include <memory>
#include <vector>

using namespace std;

class Notification {
protected:
    string message;
    string timestamp;

public:
    Notification(const string& message, const string& timestamp)
        : message(message), timestamp(timestamp) {}

    virtual void send() const {
        cout << "[" << timestamp << "] " << message << endl;
    }

    virtual ~Notification() = default;
};

class EmailNotification : public Notification {
private:
    string emailAddress;

public:
    EmailNotification(const string& message, const string& timestamp, const string& emailAddress)
        : Notification(message, timestamp), emailAddress(emailAddress) {}

    void send() const override {
        cout << "Відправка email на " << emailAddress << ": " << message << endl;
    }
};

class PushNotification : public Notification {
private:
    string deviceId;

public:
    PushNotification(const string& message, const string& timestamp, const string& deviceId)
        : Notification(message, timestamp), deviceId(deviceId) {}

    void send() const override {
        cout << "Відправка пуша на пристрій " << deviceId << ": " << message << endl;
    }
};

int main() {
    vector<unique_ptr<Notification>> notifications;
    notifications.push_back(make_unique<EmailNotification>("Ваше замовлення підтверджено", "2026-10-25 16:07", "user@example.com"));
    notifications.push_back(make_unique<PushNotification>("Нове повідомлення", "2026-10-25 16:08", "device-42"));

    for (const auto& n : notifications) {
        n->send();
    }

    return 0;
}
