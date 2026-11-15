// Q5: tickets created inside a function called twice - the static count is shared
#include <iostream>
#include <string>

class BusTicket {
    static int issued;
    static const int CAPACITY = 3;
    int seat;
    std::string name;
public:
    explicit BusTicket(const std::string& n) : seat(0), name(n)
    { if (issued < CAPACITY) seat = ++issued; }
    static int count() { return issued; }
    static int capacity() { return CAPACITY; }
    void print() const
    { std::cout << name << ": " << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n'; }
};
int BusTicket::issued = 0;

void book(const std::string& who)
{
    BusTicket t(who);              // destroyed at the end of the call
    t.print();
    std::cout << "  issued so far: " << BusTicket::count() << '\n';
}

int main()
{
    book("Asha"); book("Bala");    // first call round
    book("Chitra"); book("Dev");   // second round: count does not reset
    std::cout << "Final issued: " << BusTicket::count() << " of " << BusTicket::capacity() << '\n';
}
