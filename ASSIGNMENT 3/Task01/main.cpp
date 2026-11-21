#include <iostream>
#include <stdexcept>
#include <string>

class Book {
    std::string title;
    int copies;

    static int validCopies(int n) {
        if (n < 0) throw std::invalid_argument("Copies cannot be negative");
        return n;
    }

public:
    Book() : Book("", 0) {}
    explicit Book(const std::string& t) : Book(t, 1) {}
    Book(const std::string& t, int c) : title(t), copies(validCopies(c)) {}

    void print() const {
        std::cout << "Title: " << (title.empty() ? "(empty)" : title)
                  << ", Copies: " << copies << '\n';
    }
};

int main() {
    try {
        Book empty;
        Book donated("C++ Basics");
        Book full("Data Structures", 5);
        empty.print();
        donated.print();
        full.print();

        // Uncomment to test validation:
        // Book invalid("Invalid Book", -2);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
}
