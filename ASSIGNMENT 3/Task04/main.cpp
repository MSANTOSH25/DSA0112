#include <iomanip>
#include <iostream>

class Money {
    long long paise;

public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}

    Money operator+(const Money& other) const { return Money(0, static_cast<int>(paise + other.paise)); }
    Money operator-(const Money& other) const { return Money(0, static_cast<int>(paise - other.paise)); }
    Money& operator+=(const Money& other) { paise += other.paise; return *this; }
    bool operator==(const Money& other) const { return paise == other.paise; }
    bool operator<(const Money& other) const { return paise < other.paise; }

    friend std::ostream& operator<<(std::ostream& out, const Money& m) {
        long long absValue = m.paise < 0 ? -m.paise : m.paise;
        if (m.paise < 0) out << "-";
        out << "Rs " << absValue / 100 << '.' << std::setw(2)
            << std::setfill('0') << absValue % 100 << std::setfill(' ');
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Money& m) {
        long long rupees; int p;
        if (in >> rupees >> p) m.paise = rupees * 100 + p;
        return in;
    }
};

int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';
    Money x(0, 75), y(0, 50);
    std::cout << x + y << '\n';
    Money negative = Money(1, 0) - Money(2, 0);
    std::cout << negative << '\n';
}
