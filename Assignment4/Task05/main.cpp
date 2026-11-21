#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Cart {
    std::string items[10];
    double prices[10]{};
    int n = 0;
    double discount = 0;

public:
    Cart& add(const std::string& item, double price) {
        if (n < 10 && price >= 0) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }
};

int main() {
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);
    Cart* p = &c;
    p->add("Oil", 180);
    std::cout << std::fixed << std::setprecision(2) << p->total() << '\n';

    Cart direct;
    direct.add("Milk", 40);
    Cart* pointer = &direct;
    std::cout << pointer->total() << '\n';

    Cart& reference = direct;
    reference.add("Bread", 30);
    std::cout << reference.total() << '\n';

    Cart* dynamicCart = new Cart;
    dynamicCart->add("Apple", 50);
    std::cout << dynamicCart->total() << '\n';
    delete dynamicCart;

    Cart a, b, d;
    a.add("A", 100);
    b.add("B", 250);
    d.add("C", 175);
    Cart* carts[] = {&a, &b, &d};
    Cart* highest = carts[0];
    for (Cart* item : carts)
        if (item->total() > highest->total()) highest = item;
    std::cout << "Highest cart total: " << highest->total() << '\n';
}
