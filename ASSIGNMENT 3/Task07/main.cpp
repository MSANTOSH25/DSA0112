#include <cmath>
#include <iostream>
#include <stdexcept>

class Complex {
    double real, imag;
public:
    Complex() : real(0), imag(0) {}
    Complex(double r) : real(r), imag(0) {}
    Complex(double r, double i) : real(r), imag(i) {}

    Complex operator+(const Complex& z) const {
        return {real + z.real, imag + z.imag};
    }
    Complex operator-(const Complex& z) const {
        return {real - z.real, imag - z.imag};
    }
    Complex operator*(const Complex& z) const {
        return {real * z.real - imag * z.imag, real * z.imag + imag * z.real};
    }
    Complex operator/(const Complex& z) const {
        double denominator = z.real * z.real + z.imag * z.imag;
        if (denominator == 0.0) throw std::domain_error("Division by zero impedance");
        return {(real * z.real + imag * z.imag) / denominator,
                (imag * z.real - real * z.imag) / denominator};
    }
    bool operator==(const Complex& z) const {
        const double eps = 1e-9;
        return std::abs(real - z.real) < eps && std::abs(imag - z.imag) < eps;
    }
    double magnitude() const { return std::sqrt(real * real + imag * imag); }
    friend std::ostream& operator<<(std::ostream& out, const Complex& z) {
        out << z.real << (z.imag < 0 ? " - j" : " + j") << std::abs(z.imag);
        return out;
    }
};

int main() {
    try {
        Complex resistor(10, 0);
        Complex inductor(0, 5);
        Complex capacitor(0, -10);

        Complex series = resistor + inductor + capacitor;
        Complex parallel = (resistor * inductor) / (resistor + inductor);

        std::cout << "Series impedance: " << series << '\n';
        std::cout << "Series magnitude: " << series.magnitude() << '\n';
        std::cout << "Parallel impedance: " << parallel << '\n';

        Complex zero;
        Complex invalid = resistor / zero;
        std::cout << invalid << '\n';
    } catch (const std::domain_error& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
}
