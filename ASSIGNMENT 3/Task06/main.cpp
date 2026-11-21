#include <cstring>
#include <iostream>

class Text {
    char* data;
public:
    Text(const char* s = "") {
        data = new char[std::strlen(s) + 1];
        std::strcpy(data, s);
    }

    Text(const Text& other) {
        data = new char[std::strlen(other.data) + 1];
        std::strcpy(data, other.data);
    }

    Text& operator=(const Text& other) {
        if (this != &other) {
            char* fresh = new char[std::strlen(other.data) + 1];
            std::strcpy(fresh, other.data);
            delete[] data;
            data = fresh;
        }
        return *this;
    }

    ~Text() { delete[] data; }

    Text operator+(const Text& other) const {
        std::size_t n = std::strlen(data) + std::strlen(other.data);
        char* joined = new char[n + 1];
        std::strcpy(joined, data);
        std::strcat(joined, other.data);
        Text result(joined);
        delete[] joined;
        return result;
    }

    bool operator==(const Text& other) const { return std::strcmp(data, other.data) == 0; }
    bool operator!=(const Text& other) const { return !(*this == other); }
    friend std::ostream& operator<<(std::ostream& out, const Text& t) { return out << t.data; }
};

int main() {
    Text a("Hello "), b("World"), c("");
    Text message = a + b;
    std::cout << message << '\n';
    std::cout << std::boolalpha << (message != c) << '\n';
    message = message; // self-assignment test
    std::cout << message << '\n';
}
