// Q1: Hostel Fee Concession Updater - by value, by reference, by pointer,
// plus the "return the new fee" design for stacking concessions.
#include <iostream>
#include <string>

struct Student { std::string name; long long fee; char pad[4000]; };

// 1. by value: the parameter is a copy living on the callee's stack frame
void byValue(long long fee, int percent)
{
    fee -= fee * percent / 100;
    std::cout << "  inside byValue   : " << fee << '\n';
}
// 2. by reference: the parameter is an alias of the caller's variable
void byRef(long long& fee, int percent) { fee -= fee * percent / 100; }
// 3. by pointer: the parameter is a copy of the address; *fee is the caller's variable
void byPtr(long long* fee, int percent)
{
    if (fee) *fee -= *fee * percent / 100;
}
// const reference: read-only access to a big object, no copy
long long feeAfter(const Student& s, int percent) { return s.fee - s.fee * percent / 100; }
// return-the-new-fee design
long long discounted(long long fee, int percent) { return fee - fee * percent / 100; }

int main()
{
    long long f1 = 45000, f2 = 45000, f3 = 45000;
    std::cout << "Prediction: byValue -> 45000, byRef -> 40500, byPtr -> 40500\n";
    byValue(f1, 10);
    byRef(f2, 10);
    byPtr(&f3, 10);
    std::cout << "stored fee after byValue: " << f1 << '\n';
    std::cout << "stored fee after byRef  : " << f2 << '\n';
    std::cout << "stored fee after byPtr  : " << f3 << '\n';

    // Several concessions: merit 10%, sports 15%, single parent 25%
    long long inPlace = 45000;
    byRef(inPlace, 10); byRef(inPlace, 15); byRef(inPlace, 25);
    long long chained = discounted(discounted(discounted(45000, 10), 15), 25);
    std::cout << "stacked in place : " << inPlace << '\n';
    std::cout << "stacked by return: " << chained << '\n';

    Student s{"Asha", 45000, {}};
    std::cout << "const-ref preview (fee unchanged): " << feeAfter(s, 10)
              << " (stored " << s.fee << ")\n";
    std::cout << "sizeof(long long)=" << sizeof(long long)
              << " sizeof(Student)=" << sizeof(Student)
              << " sizeof(pointer)=" << sizeof(void*) << '\n';
}
