// Q3: Redesigned courier charge calculator
#include <iostream>

struct Parcel { double kg; bool express; bool insured; };

const double RATE_PER_KG = 50.0, EXPRESS_FEE = 40.0, INSURANCE_FEE = 20.0;

// kg as double, optional extras from the right
double charge(double kg, bool express = false, bool insured = false)
{
    std::cout << "[double,bool,bool] ";
    return kg * RATE_PER_KG + (express ? EXPRESS_FEE : 0) + (insured ? INSURANCE_FEE : 0);
}
// grams as int
double charge(int grams, bool express = false, bool insured = false)
{
    std::cout << "[int grams] ";
    return (grams / 1000.0) * RATE_PER_KG + (express ? EXPRESS_FEE : 0) + (insured ? INSURANCE_FEE : 0);
}
// Parcel object
double charge(const Parcel& p)
{
    std::cout << "[Parcel] ";
    return p.kg * RATE_PER_KG + (p.express ? EXPRESS_FEE : 0) + (p.insured ? INSURANCE_FEE : 0);
}

int main()
{
    std::cout << "Prediction: 1->int, 2->int, 3->double(float promoted), 4->double, 5->Parcel, 6->double\n";
    std::cout << "1 charge(2500)            = " << charge(2500) << '\n';
    std::cout << "2 charge(2)               = " << charge(2) << '\n';
    std::cout << "3 charge(2.0f)            = " << charge(2.0f) << '\n';
    std::cout << "4 charge(2.5, true)       = " << charge(2.5, true) << '\n';
    std::cout << "5 charge(Parcel{3,1,1})   = " << charge(Parcel{3.0, true, true}) << '\n';
    std::cout << "6 charge(1.2, false, true)= " << charge(1.2, false, true) << '\n';
}
