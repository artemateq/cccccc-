#include <iostream>
#include <stack>
#include <string>

bool isValid(const std::string& expression) {
    std::stack<char> st;

    for (char ch : expression) {
        switch (ch) {
            case '(':
            case '[':
            case '{':
                st.push(ch);
                break;

            case ')':
            case ']':
            case '}': {
                if (st.empty()) return false;

                char open = st.top();
                st.pop();

                if ((ch == ')' && open != '(') ||
                    (ch == ']' && open != '[') ||
                    (ch == '}' && open != '{')) {
                    return false;
                }
                break;
            }

            default:
                break;
        }
    }

    return st.empty();
}

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty() || line == "exit") break;
        std::cout << (isValid(line) ? "Коректно" : "Помилка") << "\n";
    }
    return 0;
}
