#pragma once
#include <cstdlib>

class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    int getH() const { return h; }
    int getM() const { return m; }
    int minutesTo(const Time& o) const { return std::abs((h * 60 + m) - (o.h * 60 + o.m)); }
    friend inline int minutesBetweenInline(const Time& a, const Time& b)
    { return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m)); }
    friend int minutesBetweenOut(const Time& a, const Time& b);   // defined in another .cpp
};
