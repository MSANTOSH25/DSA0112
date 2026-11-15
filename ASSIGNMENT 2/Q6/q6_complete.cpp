#include <iostream>
#include <cstdlib>
class Time {
int h, m;
public:
Time(int hh, int mm) : h(hh), m(mm) {}
friend int minutesBetween(const Time& a, const Time& b);
friend bool earlier(const Time& a, const Time& b);
};
int minutesBetween(const Time& a, const Time& b)
{
// TODO 1 completed
return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}
bool earlier(const Time& a, const Time& b)
{
// TODO 2 completed
return (a.h * 60 + a.m) < (b.h * 60 + b.m);
}
int main()
{
Time departure(9, 45), arrival(13, 10);
std::cout << minutesBetween(departure, arrival) << ' '
<< std::boolalpha << earlier(arrival, departure) << '\n';
// extra correctness tests
Time x(11, 50), y(12, 10), z(9, 45);
std::cout << "across noon : " << minutesBetween(x, y) << ' ' << earlier(x, y) << '\n';
std::cout << "same time   : " << minutesBetween(departure, z) << ' ' << earlier(departure, z) << '\n';
}
